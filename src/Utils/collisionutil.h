#ifndef COLLISIONUTIL_H
#define COLLISIONUTIL_H
#include <QRectF>
class CollisionUtil
{
public:
    static bool isRectCollide(const QRectF& a,const QRectF& b);
};

#endif // COLLISIONUTIL_H
