#ifndef GHOSTCELL_H
#define GHOSTCELL_H
//历史残影实体
#include <QPointF>
#include <QList>
#include "Core/gameglobal.h"
#include "monstercell.h"
class GhostCell
{
public:
    GhostCell();
    void initGhost(qreal x,qreal y,int size,GameGlobal::LifeLaw lawType);
    void update(qreal playerX,qreal playerY,const QList<MonsterCell>& monsterList);
    bool isLifeExpired(qint64 nowTime)const;
    qreal getX()const;
    qreal getY()const;
    int getSize()const;
    QColor getColor()const;
    bool checkAttackHit(const MonsterCell& mon)const;
    void setBornTime(qint64 time);
    qint64 getBornTime()const;
private:
    QPointF m_pos;
    int m_size;
    QColor m_color;
    qint64 m_bornTime;
    qreal m_moveSpeed;
};

#endif // GHOSTCELL_H
