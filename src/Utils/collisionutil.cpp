#include "collisionutil.h"
#include <cmath>
bool CollisionUtil::isRectCollide(const QRectF &a, const QRectF &b){
    return a.intersects(b);
}
bool CollisionUtil::circleCollide(qreal x1, qreal y1, int r1, qreal x2, qreal y2, int r2){
    qreal distance=hypot(x1-x2,y1-y2);
    return distance<(r1+r2);
}