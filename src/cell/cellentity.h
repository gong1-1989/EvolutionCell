#ifndef CELLENTITY_H
#define CELLENTITY_H
#include <QPointF>
#include <QRectF>
#include <QColor>
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
};

#endif // CELLENTITY_H
