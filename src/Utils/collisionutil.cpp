#include "CollisionUtil.h"

bool CollisionUtil::circleCollision(qreal x1, qreal y1, qreal r1, qreal x2, qreal y2, qreal r2)
{
    qreal dx = x1 - x2;
    qreal dy = y1 - y2;
    qreal r = r1 + r2;
    return (dx*dx+dy*dy) <= (r * r);
}