#ifndef COLLISIONUTIL_H
#define COLLISIONUTIL_H
#include <QRectF>
class CollisionUtil
{
public:
    static bool isRectCollide(const QRectF& a,const QRectF& b);
    static bool circleCollide(qreal x1,qreal y1,int r1,qreal x2,qreal y2,int r2);
};

#endif // COLLISIONUTIL_H
