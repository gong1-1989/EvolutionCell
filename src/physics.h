#ifndef PHYSICS_H
#define PHYSICS_H
#include <QRectF>
#include <QPointF>
/**
 * @brief The Physics class 细胞专用圆形碰撞检测、边界限制、混合碰撞
 */
class Physics
{
public:
    //工具类，禁止实例化
    Physics()=delete;

    /**
     * @brief circleCollision 圆和圆碰撞检测
     * @param p1 圆心1坐标
     * @param r1 圆1半径
     * @param p2 圆心2坐标
     * @param r2 圆2半径
     * @return true=发生碰撞
     */
    static bool circleCollision(const QPointF& p1,qreal r1,const QPointF& p2,qreal r2);

    /**
     * @brief circleRectCollision 圆和矩形碰撞检测
     * @param p 圆心坐标
     * @param r 圆半径
     * @param rect 目标矩形
     * @return true=发生碰撞
     */
    static bool circleRectCollision(const QPointF& p,qreal r,const QRectF& rect);

    /**
     * @brief aabbCollision AABB矩形碰撞检测
     * @param rect1 矩形1
     * @param rect2 矩形2
     * @return true=发生碰撞
     */
    static bool aabbCollision(const QRectF& rect1,const QRectF& rect2);

    /**
     * @brief pointInRect 点和矩形判断（鼠标点击、区域标记）
     * @param p 点坐标
     * @param rect 矩形
     * @return true=发生碰撞
     */
    static bool pointInRect(const QPointF& p,const QRectF& rect);

    /**
     * @brief clampCirclePos 圆形物体边界约束（圆形专用，防止细胞出界）
     * @param p 圆心坐标
     * @param radius 圆半径
     * @param winW 窗口宽度
     * @param winH 窗口高度
     * @return 约束后的合法圆心坐标
     */
    static QPointF clampCirclePos(QPointF& p,qreal radius,int winW,int winH);
};

#endif // PHYSICS_H
