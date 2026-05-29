#include "EventBus.h"
#include <QMutexLocker>
#include <QThread>

// 静态单例初始化
EventBus* EventBus::m_instance = nullptr;

EventBus* EventBus::GetInstance()
{
    // 线程安全单例创建
    static QMutex instanceMutex;
    QMutexLocker locker(&instanceMutex);

    if (m_instance == nullptr)
    {
        m_instance = new EventBus;        
        LOG_INFO(m_instance->MODULE_NAME, "事件总线单例对象创建完成");
    }
    return m_instance;
}
EventBus::EventBus():m_asyncThread(nullptr),m_throttleFrameCount(0)
{
}
EventBus::~EventBus()
{
    // 析构时停止并释放异步线程
    if (m_asyncThread != nullptr)
    {
        LOG_INFO(MODULE_NAME, "开始停止事件总线异步消费线程");
        m_asyncThread->quit();
        m_asyncThread->wait();
        delete m_asyncThread;
        m_asyncThread = nullptr;
        LOG_INFO(MODULE_NAME, "异步消费线程已安全释放");
    }
}

void EventBus::StartAsyncConsume()
{
    // 防止重复启动线程
    if (m_asyncThread != nullptr && m_asyncThread->isRunning())
    {
        LOG_WARN(MODULE_NAME, "异步消费线程已处于运行状态，无需重复启动");
        return;
    }

    m_asyncThread = new QThread(this);
    // 将消费槽函数移动到独立子线程执行
    this->moveToThread(m_asyncThread);
    // 线程启动后触发队列消费函数
    connect(m_asyncThread, &QThread::started, this, &EventBus::OnConsumeEventQueue);
    m_asyncThread->start();

    LOG_INFO(MODULE_NAME, "事件总线异步消费线程启动成功");
}

void EventBus::SubscribeEvent(PluginInterface* plugin, const QString& evtName)
{
    // 空指针防护
    if (plugin == nullptr || evtName.isEmpty())
    {
        LOG_WARN(MODULE_NAME, "订阅事件失败：插件指针为空 或 事件名称为空");
        return;
    }

    QMutexLocker locker(&m_eventMutex);
    m_subscribeMap[evtName].append(plugin);
    LOG_DBG(MODULE_NAME, QString("插件[%1] 成功订阅事件：%2")
                             .arg(plugin->GetPluginName()).arg(evtName));
}

void EventBus::UnSubscribeEvent(PluginInterface* plugin, const QString& evtName)
{
    // 空指针防护
    if (plugin == nullptr || evtName.isEmpty())
    {
        LOG_WARN(MODULE_NAME, "取消订阅失败：插件指针为空 或 事件名称为空");
        return;
    }

    QMutexLocker locker(&m_eventMutex);
    if (!m_subscribeMap.contains(evtName))
    {
        LOG_DBG(MODULE_NAME, QString("事件[%1] 无订阅记录，无需取消").arg(evtName));
        return;
    }

    auto& pluginList = m_subscribeMap[evtName];
    pluginList.removeOne(plugin);
    LOG_DBG(MODULE_NAME, QString("插件[%1] 已取消订阅事件：%2")
                             .arg(plugin->GetPluginName()).arg(evtName));

    // 订阅列表为空时，移除该事件记录（节省内存）
    if (pluginList.isEmpty())
    {
        m_subscribeMap.remove(evtName);
        LOG_DBG(MODULE_NAME, QString("事件[%1] 所有订阅者已取消，清理事件记录").arg(evtName));
    }
}

void EventBus::PublishEvent(const QString& evtName, const QVariantList& params)
{
    QMutexLocker locker(&m_eventMutex);
    // 监控队列积压，超过阈值输出警告
    if (m_eventQueue.size() > QUEUE_WARN_THRESHOLD)
    {
        LOG_WARN(MODULE_NAME, QString("事件队列积压严重，当前队列长度：%1，请检查消费效率").arg(m_eventQueue.size()));
    }

    // 普通低频事件直接入队
    m_eventQueue.enqueue({evtName, params});
    LOG_DBG(MODULE_NAME, QString("发布低频事件：%1，参数数量：%2").arg(evtName).arg(params.size()));
}

void EventBus::PublishHighFreqEvent(const QString& evtName, const QVariantList& params)
{
    // 高频事件节流：每2帧才允许一次入队，减少分发压力
    m_throttleFrameCount++;
    if (m_throttleFrameCount % 2 != 0)
    {
        // 非触发帧，直接丢弃事件（节流逻辑）
        return;
    }

    QMutexLocker locker(&m_eventMutex);
    if (m_eventQueue.size() > QUEUE_WARN_THRESHOLD)
    {
        LOG_WARN(MODULE_NAME, QString("事件队列积压严重，当前队列长度：%1").arg(m_eventQueue.size()));
    }

    m_eventQueue.enqueue({evtName, params});
    LOG_DBG(MODULE_NAME, QString("发布高频事件(已节流)：%1").arg(evtName));
}

void EventBus::OnConsumeEventQueue()
{
    LOG_INFO(MODULE_NAME, "事件队列消费循环开始运行");

    // 无限循环消费队列（子线程常驻）
    while (true)
    {
        QMutexLocker locker(&m_eventMutex);
        // 队列为空，释放锁并休眠1ms，降低CPU占用
        if (m_eventQueue.isEmpty())
        {
            locker.unlock();
            QThread::msleep(1);
            continue;
        }

        // 取出队首事件
        QPair<QString, QVariantList> eventData = m_eventQueue.dequeue();
        QString evtName = eventData.first;
        QVariantList params = eventData.second;
        locker.unlock();

        // 无任何插件订阅该事件，直接跳过
        if (!m_subscribeMap.contains(evtName))
        {
            LOG_DBG(MODULE_NAME, QString("事件[%1] 无订阅者，跳过分发").arg(evtName));
            continue;
        }

        // 定向分发：仅遍历订阅当前事件的插件
        QList<PluginInterface*>& pluginList = m_subscribeMap[evtName];
        for (PluginInterface* plugin : pluginList)
        {
            if (plugin == nullptr)
            {
                LOG_WARN(MODULE_NAME, QString("事件[%1] 订阅列表存在空指针插件，跳过").arg(evtName));
                continue;
            }

            // 根据事件名称，映射到对应Qt信号（严格匹配策划EVT_*定义）
            if (evtName == "EVT_MIGRATE" && params.size() >= 3)
            {
                quint64 cellId = params[0].toULongLong();
                int fromLayer = params[1].toInt();
                int toLayer = params[2].toInt();
                emit EVT_MIGRATE(cellId, fromLayer, toLayer);
            }
            else if (evtName == "EVT_HGT" && params.size() >= 1)
            {
                emit EVT_HGT(params[0].toULongLong());
            }
            else if (evtName == "EVT_BIOFILM_BREAK" && params.size() >= 1)
            {
                emit EVT_BIOFILM_BREAK(params[0].toULongLong());
            }
            else if (evtName == "EVT_GAME_OVER" && params.size() >= 1)
            {
                auto level = static_cast<Global::GameOverLevel>(params[0].toInt());
                emit EVT_GAME_OVER(level);
            }
            else if (evtName == "EVT_HABITAT_CRASH" && params.size() >= 1)
            {
                emit EVT_HABITAT_CRASH(params[0].toInt());
            }
            else if (evtName == "EVT_CHEMOTAXIS_SIGNAL" && params.size() >= 3)
            {
                int type = params[0].toInt();
                QPointF pos = params[1].toPointF();
                int radius = params[2].toInt();
                emit EVT_CHEMOTAXIS_SIGNAL(type, pos, radius);
            }
            else
            {
                LOG_DBG(MODULE_NAME, QString("事件[%1] 无匹配信号，参数不完整").arg(evtName));
            }
        }
    }
}