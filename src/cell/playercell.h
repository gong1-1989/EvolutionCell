#ifndef PLAYERCELL_H
#define PLAYERCELL_H
//玩家实体类
#include <QPointF>
#include "Core/gameglobal.h"
#include "gene.h"
#include "symbiosiscell.h"
class PlayerCell
{
public:
    PlayerCell();
    void move(bool w,bool a,bool s,bool d,int canvasW,int canvasH);
    void grow(int addSize,GameGlobal::MonsterType type);
    void applySpeedBuff();
    void applySpeedDebuff();
    void clearTempEffect();

    //开局选定生命法则，决定基础属性偏向
    void initLifeLaw(GameGlobal::LifeLaw law);
    //躯体解构核心方法
    bool doDecompose(GameGlobal::DecomposeLevel targetLv);
    qreal getCritBonus()const;
    qreal getAttackBouns()const;
    qreal getSpeedModify()const;
    GameGlobal::LifeLaw getCurrentLaw()const;
    GameGlobal::DecomposeLevel getDecomposeLevel()const;

    qreal getX()const;
    qreal getY()const;
    int getSize()const;
    void setPos(qreal x,qreal y);
    bool hasSpeedBuff()const;
    int getDecomposeRisk()const;

    bool unlockGene(GameGlobal::GeneType type);
    qreal getGeneRangeRatio()const;
    qreal getGeneSpeedRatio()const;
    qreal getGeneGrowRatio()const;
    const QList<Gene>& getUnlockedGene()const;

    bool addSymbiosisCell(const SymbiosisCell& cell);
    void clealExpiredSymbiosis(qint64 nowTime);
    GameGlobal::RejectLevel getcurrentRejectLevel()const;
    qreal getRejectAttrModify()const;
    int getSymbiosisCount()const;
    const QList<SymbiosisCell>& getSymbiosisList()const;
    QList<SymbiosisCell>& getMutableSymbiosisList();
    int getGeneRejectValue()const;

    void resetSymbiosis();
    void setGenRejectValue(int value);

private:
    qreal m_x,m_y,m_speed;
    int m_size;
    bool m_hasSpeedBuff;
    bool m_hasSpeedDebuff;
    QList<Gene> m_unlockGene;

    GameGlobal::LifeLaw m_lawType;
    GameGlobal::DecomposeLevel m_decomposeLv;
    int m_currentDecomposeRisk;
    qreal m_critBonus;
    qreal m_atkBonus;
    qreal m_speedLoss;
    int m_symbiosisCount;

    QList<SymbiosisCell> m_symbiosisList;
    int m_geneRejectValue;
};

#endif // PLAYERCELL_H
