#ifndef PLAYERCELL_H
#define PLAYERCELL_H
//玩家实体类
#include <QPointF>
#include "Core/gameglobal.h"
#include "gene.h"
class PlayerCell
{
public:
    PlayerCell();
    void move(bool w,bool a,bool s,bool d,int canvasW,int canvasH);
    void grow(int addSize,GameGlobal::MonsterType type);
    void applySpeedBuff();
    void applySpeedDebuff();
    void clearTempEffect();

    qreal getX()const;
    qreal getY()const;
    int getSize()const;
    void setPos(qreal x,qreal y);
    bool hasSpeedBuff()const;

    bool unlockGene(GameGlobal::GeneType type);
    qreal getGeneRangeRatio()const;
    qreal getGeneSpeedRatio()const;
    qreal getGeneGrowRatio()const;
    const QList<Gene>& getUnlockedGene()const;

private:
    qreal m_x,m_y,m_speed;
    int m_size;
    bool m_hasSpeedBuff;
    bool m_hasSpeedDebuff;
    QList<Gene> m_unlockGene;
};

#endif // PLAYERCELL_H
