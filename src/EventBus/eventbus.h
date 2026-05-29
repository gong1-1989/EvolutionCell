#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <QObject>
#include <QPointF>
#include <QQueue>
#include <QThread>
#include <QHash>
#include <QMutex>
#include <QVariantList>
#include "Global/GlobalDefine.h"
#include "Global/LogTool.h"
#include "PluginInterface/PluginInterface.h"

/**
 * @brief 优化版全局异步事件总线 【静态内核】
 * 架构说明：
 * 1. 全局模块通信中枢，所有内核模块、DLL插件统一通过本模块交互
 * 2. 永久静态编译，不拆分为DLL；独立异步线程消费事件，不阻塞60FPS主循环
 * 3. 核心优化（解决海量细胞模拟广播风暴）：
 *    - 定向订阅：仅向已订阅插件分发事件，摒弃全局广播
 *    - 异步队列：事件入队后由独立线程处理，主线程无阻塞
 *    - 高频节流：细胞类高频事件每2帧仅触发一次，降低CPU开销
 * 4. 新增能力：全流程中文日志、队列积压监控、空指针防护、线程状态日志
 * 事件规范：严格遵循策划定义的 EVT_* 标准事件名
 */
class EventBus : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 获取全局单例（线程安全）
     */
    static EventBus* GetInstance();

    /**
     * @brief 启动异步消费线程
     * 调用时机：程序初始化后期、插件加载完成后执行
     */
    void StartAsyncConsume();

    // ===================== 订阅/取消订阅（定向分发核心） =====================
    /**
     * @brief 插件/模块 订阅指定事件
     * @param plugin 插件实例指针
     * @param evtName 事件名称（标准EVT_*）
     */
    void SubscribeEvent(PluginInterface* plugin, const QString& evtName);

    /**
     * @brief 插件/模块 取消指定事件订阅
     * @param plugin 插件实例指针
     * @param evtName 事件名称（标准EVT_*）
     */
    void UnSubscribeEvent(PluginInterface* plugin, const QString& evtName);

    // ===================== 事件发布接口 =====================
    /**
     * @brief 发布低频全局事件（对局结束、生境崩溃、UI弹窗等）
     * 无节流限制，实时入队分发
     * @param evtName 事件名
     * @param params 事件携带参数
     */
    void PublishEvent(const QString& evtName, const QVariantList& params = QVariantList());

    /**
     * @brief 发布高频细胞事件（演化、迁徙、基因转移等）
     * 自动节流：每2帧仅允许一次入队，抑制高频刷屏
     * @param evtName 事件名
     * @param params 事件携带参数
     */
    void PublishHighFreqEvent(const QString& evtName, const QVariantList& params = QVariantList());

signals:
    // ===================== 标准事件列表（完全对标策划 EVT_* 定义） =====================
    void EVT_AREA_MARK(int markType, QPointF pos);                // 区域标记事件
    void EVT_CHEMOTAXIS_SIGNAL(int signalType, QPointF pos, int radius); // 趋化引导信号
    void EVT_CELL_EVOLVE(quint64 cellId, Global::DifferLevel level);    // 细胞演化事件
    void EVT_CELL_DEATH(quint64 cellId);                                // 细胞死亡事件
    void EVT_MIGRATE(quint64 cellId, int fromLayer, int toLayer);       // 跨层迁徙（联动科普弹窗）
    void EVT_ECO_EVENT(int eventId);                                   // 随机生态事件
    void EVT_HGT(quint64 cellId);                                      // 水平基因转移（科普弹窗）
    void EVT_BIOFILM_BREAK(quint64 cellId);                             // 生物膜解体（科普弹窗）
    void EVT_HABITAT_CRASH(int layerId);                                // 局部生境崩溃
    void EVT_GAME_OVER(Global::GameOverLevel level);                    // 全域对局结束

private slots:
    /**
     * @brief 异步队列消费主函数（运行在独立子线程）
     * 循环读取事件队列，定向分发给所有订阅者
     */
    void OnConsumeEventQueue();

private:
    // 私有构造&析构：单例模式，禁止外部实例化
    EventBus();
    ~EventBus() override;
    static EventBus* m_instance;

    const QString MODULE_NAME = "事件总线";

    // 线程安全互斥锁：保护订阅表、事件队列（多线程读写防错乱）
    QMutex m_eventMutex;

    // 订阅映射表：Key=事件名  Value=订阅该事件的插件列表
    QHash<QString, QList<PluginInterface*>> m_subscribeMap;

    // 异步事件队列：存储待分发事件 + 对应参数
    QQueue<QPair<QString, QVariantList>> m_eventQueue;

    // 异步消费线程对象
    QThread* m_asyncThread;

    // 高频事件节流计数器：每2帧触发一次
    int m_throttleFrameCount;

    // 队列积压告警阈值：队列长度超过该值输出警告日志
    static constexpr int QUEUE_WARN_THRESHOLD = 1000;
};

#endif // EVENTBUS_H