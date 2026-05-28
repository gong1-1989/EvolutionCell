#ifndef GAMECANVAS_H
#define GAMECANVAS_H

#include <QMainWindow>
#include <QPainter>
#include <QDebug>
#include "gametimer.h"
#include "jsonparser.h"
#include "eventbus.h"
#include "Cell/cell.h"

/**
 * @brief The GameCanvas class 游戏渲染布局，视图管理、帧循环绑定
 */
class GameCanvas : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameCanvas(QWidget *parent = nullptr);
    ~GameCanvas() override;

protected:
    //重写绘制事件：游戏画面渲染入口
    void paintEvent(QPaintEvent *event) override;
private slots:
    //全局帧更新回调：驱动所有游戏逻辑
    void onFrameUpdate();
private:
    GameTimer * m_gameTimer=nullptr;
    QPainter* m_painter;
    const int m_winWidth=1280;
    const int m_winHeight=720;

    Cell* m_testCell;//测试细胞实例
    QJsonObject m_cellBaseConfig;
};

#endif // GAMECANVAS_H
