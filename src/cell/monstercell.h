#ifndef MONSTERCELL_H
#define MONSTERCELL_H
//怪物实体类
#include <QColor>
#include "Core/gameglobal.h"
class MonsterCell
{
public:
    MonsterCell();
    void initRandom(qreal limitW,qreal limitH,qreal playerX,qreal playerY);
    void move(int canvasW,int canvasH);
    qreal getX()const;
    qreal getY()const;
    int getSize()const;
    QColor getColor()const;
    GameGlobal::MonsterType getType()const;
private:
    qreal m_x,m_y,m_vx,m_vy;
    int m_size;
    QColor m_color;
    GameGlobal::MonsterType m_type;
};

#endif // MONSTERCELL_H
