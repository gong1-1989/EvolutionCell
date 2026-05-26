#ifndef GHOSTCELL_H
#define GHOSTCELL_H

#include "Core/GameGlobal.h"
#include "MonsterCell.h"
#include "Utils/CollisionUtil.h"
#include <QPointF>
#include <QColor>
#include <QList>
#include <QJsonObject>
#include <cmath>

/**
 * @brief 时空残影实体
 * 生成规则：玩家执行时空回溯后创建
 * 行为：主动寻敌攻击、跟随玩家、限时自动销毁
 */
class GhostCell
{
public:
    GhostCell();

    /**
     * @brief 初始化残影
     * @param x 坐标X
     * @param y 坐标Y
     * @param size 体型
     * @param lawType 生命法则（决定残影颜色）
     */
    void initGhost(qreal x, qreal y, int size, GameGlobal::LifeLaw lawType);

    /**
     * @brief 帧更新：寻敌 + 移动
     * @param playerX 玩家X
     * @param playerY 玩家Y
     * @param monsterList 全场怪物列表
     */
    void update(qreal playerX, qreal playerY, const QList<MonsterCell>& monsterList);

    /**
     * @brief 判断残影生命周期是否过期
     * @param nowTime 当前时间戳
     * @return true=已过期，需要销毁
     */
    bool isLifeExpired(qint64 nowTime) const;

    /**
     * @brief 碰撞检测：残影是否命中怪物
     */
    bool checkAttackHit(const MonsterCell& mon) const;

    // 只读查询接口
    qreal getX() const;
    qreal getY() const;
    int getSize() const;
    QColor getColor() const;
    qint64 getBornTime() const;

    // 状态设置
    void setBornTime(qint64 time);

    // ===================== 序列化接口 =====================
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& obj);

private:
    QPointF m_pos;               // 坐标
    int m_size;                  // 体型
    QColor m_color;              // 半透明颜色
    qint64 m_bornTime;           // 生成时间（生命周期计时）
    qreal m_moveSpeed;           // 移动速度
    GameGlobal::LifeLaw m_law;   // 绑定的生命法则
};

#endif // GHOSTCELL_H