#ifndef COLLISIONUTIL_H
#define COLLISIONUTIL_H
#include <QtGlobal>
#include <cmath>

/**
 * @brief 碰撞检测工具类
 * 圆形碰撞（细胞专用）
 */
class CollisionUtil
{
public:
    static bool circleCollision(qreal x1, qreal y1, qreal r1,
                                qreal x2, qreal y2, qreal r2);
};

#endif // COLLISIONUTIL_H