#ifndef GAMECANVAS_H
#define GAMECANVAS_H
//自定义绘图画布类
#include <QWidget>
#include <QPaintEvent>
#include <QTimer>
#include <QKeyEvent>
#include "Core/gamemanager.h"

class GameCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit GameCanvas(QWidget *parent = nullptr);
    void reserGameCanvas();
    void loadGameBySlot(const QString&path);
protected:
    void paintEvent(QPaintEvent *e)override;
    void keyPressEvent(QKeyEvent *e)override;
    void keyReleaseEvent(QKeyEvent *e)override;

private slots:
    void updateCellPos();
private:
    bool isinitializd=false;
    bool k_w,k_s,k_a,k_d;
    QTimer* refreshTimer;
    GameManager m_gameMgr;
};

#endif // GAMECANVAS_H
