#include "cellentity.h"

Cellentity::Cellentity() {
    m_pos=QPointF(400,300);
    m_radius=16;
    m_moveSpeed=120.0f;
    m_cellcolor=QColor(220,60,60);
    m_hp=100;
    m_attack=10;
    m_attackRange=45;
    m_attackCd=0.8f;
    m_cdTimer=0.0f;
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

void Cellentity::cellAttack(const QList<Cellentity> &cellList, float deltaTime){
    m_cdTimer+=deltaTime;
    if(m_cdTimer<m_attackCd) return;
    for(Cellentity &cell:cellList){
        if(MathAToB::aToB(m_pos,cell.m_pos)<=m_attackRange){
            cell.m_hp-=m_attack;
            m_cdTimer=0;
            break;
        }
    }
}
void Cellentity::moveChase(QPointF targetPos, const QList<Cellentity> &cellList, float deltaTime){
    for(Cellentity& cell:cellList){
        QPointF pos=MathAToB::AMoveFromB(targetPos,cell.m_pos);
        moveCell(pos.x()*m_moveSpeed*deltaTime,pos.y()*m_moveSpeed*deltaTime);
    }
}
