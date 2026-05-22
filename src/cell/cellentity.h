#ifndef CELLENTITY_H
#define CELLENTITY_H
#include <QPointF>
#include <QRectF>
#include <QColor>
#include <QList>
#include "../Utils/MathAToB.h"
class Cellentity
{
public:
    Cellentity();
    void moveCell(float dx,float dy);
    QRectF getCellRect()const;
    void limitBoundary(const QRectF& bound);

    QPointF m_pos;
    qreal m_radius;
    qreal m_moveSpeed;
    QColor m_cellcolor;

    int m_hp=0;
    int m_attack=0;
    qreal m_attackRange=0.0;
    qreal m_attackCd=0.0;
    qreal m_cdTimer=0.0;

    void cellAttack(const QList<Cellentity>& cellList, float deltaTime);
    void moveChase(QPointF targetPos,const QList<Cellentity>& cellList,float deltaTime);
private:
    QList<Cellentity> m_cellList;
};

#endif // CELLENTITY_H
