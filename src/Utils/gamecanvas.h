#ifndef GAMECANVAS_H
#define GAMECANVAS_H

#include "Core/GameRender.h"
#include "Core/GameGlobal.h"
#include "Core/GameManager.h"
#include "Utils/savemanager.h"
#include <QWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QJsonArray>
#include <QPainter>
#include <QDebug>

/**
 * @brief 游戏画布窗口
 * 优化点：
 * 1. 按键防抖，防止帧循环连点触发
 * 2. 按键逻辑与业务解耦，仅转发指令
 * 3. 区分运行/暂停状态按键规则
 * 4. 固定画布尺寸
 */
class GameCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit GameCanvas(QWidget *parent = nullptr);    

protected:
    // 重写绘制、键盘事件
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    // 帧刷新定时器
    void onFrameUpdate();
    // 按键防抖定时器
    void resetKeyState();

private:   

    // 帧定时器（60FPS）
    QTimer* m_frameTimer;
    // 按键防抖定时器
    QTimer* m_keyDebounceTimer;

    // 按键状态标记（WASD）
    bool m_keyW = false;
    bool m_keyA = false;
    bool m_keyS = false;
    bool m_keyD = false;

    // 防抖标记（防止快捷键连点）
    bool m_keyLocked = false;

    // 绘制器实例
    GameRender m_render;
};

#endif // GAMECANVAS_H