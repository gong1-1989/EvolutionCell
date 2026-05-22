#include "cellentity.h"

Cellentity::Cellentity() {
    m_pos=QPointF(400,300);
    m_radius=16;
    m_moveSpeed=120.0f;
    m_cellcolor=QColor(220,60,60);
}
void Cellentity::moveCell(float dx, float dy){
    m_pos.rx()+=dx;
    m_pos.ry()+=dy;
}
QRectF Cellentity::getCellRect() const{
    return QRectF(m_pos.x()-m_radius,m_pos.y()-m_radius,m_radius*2,m_radius*2);
}
void Cellentity::limitBoundary(const QRectF &bound){
    QRectF cellRect=getCellRect();
    if(cellRect.left()<bound.left()) m_pos.setX(bound.left()+m_radius);
    if(cellRect.right()>bound.right()) m_pos.setX(bound.right()-m_radius);
    if(cellRect.top()<bound.top()) m_pos.setY(bound.top()+m_radius);
    if(cellRect.bottom()>bound.bottom()) m_pos.setY(bound.bottom()-m_radius);
}