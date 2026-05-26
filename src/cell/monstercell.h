#ifndef MONSTERCELL_H
#define MONSTERCELL_H

#include "Core/GameGlobal.h"
#include "Utils/RandomUtil.h"
#include <QPointF>
#include <QColor>
#include <QJsonObject>
#include <cmath>
#include <QDebug>

/**
 * @brief 怪物实体类
 * 功能：随机生成、边界反弹移动、属性区分(普通/精英/特殊)、序列化存档
 * 封装规则：私有成员仅内部访问，外部通过公有接口读写
 */
class MonsterCell
{
public:
    MonsterCell();

    /**
     * @brief 初始化怪物属性与位置
     * @param type 怪物类型
     * @param canvasW 画布宽度
     * @param canvasH 画布高度
     */
    void initMonster(GameGlobal::MonsterType type, int canvasW, int canvasH);

    /**
     * @brief 怪物帧更新：移动 + 边界反弹
     * @param canvasW 画布宽
     * @param canvasH 画布高
     * @param playerX 玩家位置X，默认为-1
     * @param playerY 玩家位置Y，默认为-1
     */
    void move(int canvasW, int canvasH,qreal playerX=-1,qreal playerY=-1);

    // 只读查询接口（渲染、碰撞判定使用）
    qreal getX() const;
    qreal getY() const;
    int getSize() const;
    QColor getColor() const;
    GameGlobal::MonsterType getType() const;

    // ===================== 序列化接口（存档专用） =====================
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& obj);

    //====================== 怪物AI接口 ==============================
    /**
     * @brief 是否追击
     * @return=true为在追击
     */
    bool isChasing();
    /**
     * @brief 设置追击状态
     * @param chase 追击状态
     */
    void setChase(bool chase);
    /**
     * @brief 获取感知范围
     * @return 感知范围
     */
    qreal getDetectRange();
    /**
     * @brief 设置感知范围
     * @param range 感知范围
     */
    void setDetecRange(qreal range);

private:
    GameGlobal::MonsterType m_type; // 怪物类型
    QPointF m_pos;                  // 坐标
    int m_size;                     // 体型大小
    QColor m_color;                 // 绘制颜色
    qreal m_speed;                  // 移动速度
    qreal m_dirX;                   // X方向分量
    qreal m_dirY;                   // Y方向分量
    //怪物AI状态
    bool m_isChasing;               //是否正在追击玩家
    qreal m_detectRange;            //感知范围（进入此范围会被追击）
};

#endif // MONSTERCELL_H