#ifndef GAMETIMER_H
#define GAMETIMER_H

#include <QObject>
#include <QTimer>
/**
 * @brief The GameTimer class 全局游戏帧循环定时器，统一控制60FPS主循环
 */
class GameTimer : public QObject
{
    Q_OBJECT
public:
    explicit GameTimer(QObject *parent = nullptr);
    //启动全局游戏主循环
    void startGameLoop();
    //停止全局游戏主循环
    void stopGameLoop();
signals:
    //每帧触发的信号（所有逻辑/渲染绑定此信号）
    void frameUpdate();
private:
    QTimer* m_frameTimer=nullptr;
    const int m_targetFps;
};

#endif // GAMETIMER_H
