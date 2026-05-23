#ifndef GAMERENDER_H
#define GAMERENDER_H
//渲染模块：纯绘制逻辑
#include <QPainter>
#include <QRect>
#include "Core/gamemanager.h"
class GameRender
{
public:
    static void drawAll(QPainter* p,const GameManager& gameMgr,const QRect&canvasRect);
};

#endif // GAMERENDER_H
