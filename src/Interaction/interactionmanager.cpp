#include "InteractionManager.h"

InteractionManager* InteractionManager::m_instance = nullptr;

InteractionManager* InteractionManager::GetInstance()
{
    static QMutex instMutex;
    QMutexLocker locker(&instMutex);

    if (m_instance == nullptr)
    {
        m_instance = new InteractionManager;
        LOG_INFO(m_instance->MODULE_NAME, "交互管理器单例创建成功");
    }
    return m_instance;
}

void InteractionManager::InitInteraction()
{
    // 程序默认：旁观模式，无交互权重
    m_currentMode = Global::InteractMode::Watch;
    m_interactWeight = 0.0;
    LOG_INFO(MODULE_NAME, "交互系统初始化完成，默认模式：旁观");
}

void InteractionManager::SwitchInteractMode(Global::InteractMode mode)
{
    m_currentMode = mode;
    QString modeText;

    // 按策划规则赋值交互权重
    switch (mode)
    {
    case Global::InteractMode::Watch:
        m_interactWeight = 0.0;
        modeText = "旁观模式";
        break;
    case Global::InteractMode::Chemotaxis:
        m_interactWeight = 0.4;
        modeText = "趋化引导模式";
        break;
    case Global::InteractMode::DeepInterfere:
        m_interactWeight = 0.6;
        modeText = "深度干预模式";
        break;
    default:
        m_interactWeight = 0.0;
        modeText = "未知模式，回退为旁观";
        LOG_WARN(MODULE_NAME, "切换到非法交互模式，强制回退为旁观");
        m_currentMode = Global::InteractMode::Watch;
        break;
    }

    LOG_INFO(MODULE_NAME, QString("交互模式切换为：%1，当前权重：%2").arg(modeText).arg(m_interactWeight));
}

double InteractionManager::GetCurrentInteractWeight() const
{
    return m_interactWeight;
}

void InteractionManager::OnMouseClick(QPointF pos, Qt::MouseButton button)
{
    // 仅趋化引导模式响应鼠标点击
    if (m_currentMode != Global::InteractMode::Chemotaxis)
        return;

    // 仅响应左键，过滤右键/中键
    if (button != Qt::LeftButton)
        return;

    // 发布趋化信号高频事件：参数(信号类型, 坐标, 作用半径)
    QVariantList params;
    params << 1 << pos << 60;
    EventBus::GetInstance()->PublishHighFreqEvent("EVT_CHEMOTAXIS_SIGNAL", params);
    LOG_DBG(MODULE_NAME, QString("鼠标点击生成趋化信号，坐标：%1,%2").arg(pos.x(), 0, 'f', 1).arg(pos.y(), 0, 'f', 1));
}

void InteractionManager::OnKeyPress(int keyCode)
{
    // 快捷键规则：1=旁观 2=趋化 3=深度干预
    switch (keyCode)
    {
    case Qt::Key_1:
        SwitchInteractMode(Global::InteractMode::Watch);
        break;
    case Qt::Key_2:
        SwitchInteractMode(Global::InteractMode::Chemotaxis);
        break;
    case Qt::Key_3:
        SwitchInteractMode(Global::InteractMode::DeepInterfere);
        break;
    default:
        // 其他按键不处理
        break;
    }
}

void InteractionManager::OnMouseMove(QPointF pos)
{
    // 预留接口：视角拖拽、跟随等拓展功能，交由上层插件实现
    Q_UNUSED(pos);
}