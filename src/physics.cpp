#include "physics.h"
#include <QtMath>

bool Physics::circleCollision(const QPointF &p1, qreal r1, const QPointF &p2, qreal r2){
    //计算两点X，Y轴差值
    qreal dx=p1.x()-p2.x();
    qreal dy=p1.y()-p2.y();
    //圆心距平方（避免开方运算，提高性能）
    qreal distSq=dx*dx+dy*dy;
    //两圆半径和的平方
    qreal rSum=r1+r2;
    qreal rSumSq=rSum*rSum;
    //距离平方<半径和平方👉碰撞
    return distSq<rSumSq;
}

bool Physics::circleRectCollision(const QPointF &p, qreal r, const QRectF &rect){
    //寻找矩形上离圆心最近的点
    qreal nearX=qBound(rect.left(),p.x(),rect.right());
    qreal nearY=qBound(rect.top(),p.y(),rect.bottom());
    //计算最近点与圆心的距离平方
    qreal dx=p.x()-nearX;
    qreal dy=p.y()-nearY;
    qreal distSq=dx*dx+dy*dy;
    return distSq<(r*r);
}

bool Physics::aabbCollision(const QRectF &rect1, const QRectF &rect2){
    return rect1.contains(rect2);
}

bool Physics::pointInRect(const QPointF &p, const QRectF &rect){
    return rect.contains(p);
}

QPointF Physics::clampCirclePos(QPointF &p, qreal radius, int winW, int winH){
    if(p.x()<radius) p.setX(radius);
    if(p.x()>winW-radius) p.setX(winW-radius);
    if(p.y()<radius) p.setY(radius);
    if(p.y()>winH-radius) p.setY(winH-radius);
    return p;
}