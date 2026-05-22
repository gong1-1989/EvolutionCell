#include "collisionutil.h"

bool CollisionUtil::isRectCollide(const QRectF &a, const QRectF &b){
    return a.intersects(b);
}
