#ifndef SYMBIOSISCELL_H
#define SYMBIOSISCELL_H
#include <QPointF>
#include "Core/gameglobal.h"
/**
 * @brief 共生菌体实体
 * 支持临时寄生、永久共生、吞噬同化三种模式
 * 具备跟随本体、生命周期过期销毁逻辑
 */

class SymbiosisCell
{
public:
    SymbiosisCell();
    //初始化共生体，继承被吞噬怪物的基础属性
    void initSymbiosis(GameGlobal::MonsterType monType,qreal x,qreal y);
    //帧更新：跟随玩家本体移动
    void followUpdate(qreal playerX,qreal playerY,qreal followRange);
    //判定临时寄生是否过期
    bool isTempExpired(qint64 currentTime)const;
    //获取基础属性
    qreal getX()const;
    qreal getY()const;
    int getSize()const;
    QColor getColor()const;
    GameGlobal::SymbiosisMode getMode()const;
    //设置共生模式
    void setSymbiosisMode(GameGlobal::SymbiosisMode mode);
    //记录寄生生效时间
    void setBornTime(qint64 time);
private:
    GameGlobal::SymbiosisMode m_mode;
    GameGlobal::MonsterType m_sourceType;
    QPointF m_pos;
    int m_size;
    QColor m_color;
    qint64 m_bornTime;
};

#endif // SYMBIOSISCELL_H
