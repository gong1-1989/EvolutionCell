#ifndef INTERACTIONMANAGER_H
#define INTERACTIONMANAGER_H

#include <QObject>
#include <QPointF>
#include <Qt>
#include "Global/GlobalDefine.h"
#include "Global/LogTool.h"
#include "EventBus/EventBus.h"

/**
 * @brief 底层键鼠交互管理器 【静态内核】
 * 架构说明：
 * 1. 底层按键映射、交互权重、模式规则永久静态
 * 2. 上层引导UI、自定义拓展交互逻辑拆分至 InteractionPlugin 插件
 * 对标策划：第四章 三层交互模式、全局键鼠快捷键规范
 * 规则：
 * - 按键1：旁观模式(权重0) | 按键2：趋化引导(权重0.4) | 按键3：深度干预(权重0.6)
 * - 仅趋化模式响应鼠标点击，生成趋化信号事件
 */
class InteractionManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 获取全局单例
     */
    static InteractionManager* GetInstance();

    /**
     * @brief 初始化交互系统（默认进入旁观模式）
     */
    void InitInteraction();

    /**
     * @brief 切换玩家交互模式
     * @param mode 目标交互模式枚举
     */
    void SwitchInteractMode(Global::InteractMode mode);

    /**
     * @brief 获取当前交互权重 0.0 ~ 0.6
     */
    double GetCurrentInteractWeight() const;

    // ===================== 原始输入事件回调（主窗口转发） =====================
    /**
     * @brief 鼠标点击事件
     * @param pos 窗口坐标
     * @param button 鼠标按键
     */
    void OnMouseClick(QPointF pos, Qt::MouseButton button);

    /**
     * @brief 键盘按键按下事件
     * @param keyCode Qt按键编码
     */
    void OnKeyPress(int keyCode);

    /**
     * @brief 鼠标移动事件
     * @param pos 窗口坐标
     */
    void OnMouseMove(QPointF pos);

private:
    InteractionManager() = default;
    static InteractionManager* m_instance;
    const QString MODULE_NAME = "交互管理器";

    Global::InteractMode m_currentMode;  // 当前交互模式
    double m_interactWeight;             // 当前交互权重
};

#endif // INTERACTIONMANAGER_H