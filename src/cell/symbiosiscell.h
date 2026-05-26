#ifndef SYMBIOSISCELL_H
#define SYMBIOSISCELL_H

#include "monstercell.h"
#include "Core/GameGlobal.h"
#include "Utils/CollisionUtil.h"
#include "monstercell.h"
#include <QPointF>
#include <QList>
#include <cmath>
#include <QDateTime>
#include <QJsonObject>
#include <QDebug>

/**
 * @brief 共生菌体实体
 * 功能：跟随玩家、主动寻敌攻击、限时销毁、序列化存档
 * 封装原则：外部禁止直接修改内部状态，仅通过公有接口操作
 */
class SymbiosisCell
{
public:
    SymbiosisCell();

    // 初始化共生体
    void initSymbiosis(GameGlobal::MonsterType monType, qreal x, qreal y);
    // 统一更新：寻敌 + 移动 + 跟随
    void update(qreal playerX, qreal playerY, qreal followRange, const QList<MonsterCell>& monsterList);

    // 状态判断
    bool isTempExpired(qint64 currentTime) const;
    bool canAttack(qint64 currentTime) const;
    void onAttackTriggered(qint64 currentTime);

    // 只读查询接口（渲染/HUD用）
    qreal getX() const;
    qreal getY() const;
    int getSize() const;
    QColor getColor() const;
    GameGlobal::SymbiosisMode getMode() const;
    GameGlobal::MonsterType getSourceType() const;
    qint64 getBornTime() const;
    qint64 getLastAttackTime() const;

    // 状态设置接口
    void setSymbiosisMode(GameGlobal::SymbiosisMode mode);
    void setBornTime(qint64 time);
    void setLastAttackTime(qint64 time);

    // ===================== 序列化接口（存档专用） =====================
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& obj);

private:
    GameGlobal::SymbiosisMode m_mode;        // 共生模式
    GameGlobal::MonsterType m_sourceType;   // 来源怪物类型
    QPointF m_pos;                           // 坐标
    int m_size;                              // 尺寸
    QColor m_color;                          // 颜色
    qint64 m_bornTime;                       // 生成时间
    qint64 m_lastAttackTime;                // 上次攻击时间（冷却）
    qreal m_moveSpeed;                       // 移动速度（取自全局常量）
};

#endif // SYMBIOSISCELL_H