#include "ghostcell.h"
#include "Utils/collisionutil.h"
#include <cmath>
#include<QDateTime>

GhostCell::GhostCell():m_size(10),m_bornTime(0),m_moveSpeed(2.0) {}


void GhostCell::initGhost(qreal x,qreal y,int size,GameGlobal::LifeLaw lawType){
    m_pos={x,y};
    m_size=size;
    switch (lawType) {
    case GameGlobal::LAW_FISSION:
        m_color=QColor(255,80,80,120);
        break;
    case GameGlobal::LAW_SYMBIOSIS:
        m_color=QColor(80,255,120,120);
        break;
    case GameGlobal::LAW_ILLUSION:
        m_color=QColor(160,80,255,120);
        break;
    }
}
void GhostCell::update(qreal playerX,qreal playerY,const QList<MonsterCell>& monsterList){
    const MonsterCell* target=nullptr;
    qreal minDist=99999.9,dx=0,dy=0;
    for(const MonsterCell& mon:monsterList){
        dx=mon.getX()-m_pos.x();
        dy=mon.getY()-m_pos.y();
        qreal dist=sqrt(dx*dx+dy*dy);
        if(dist<minDist){
            minDist=dist;
            target=&mon;
        }
    }
    if(target){
        dx=target->getX()-m_pos.x();
        dy=target->getY()-m_pos.y();
        qreal dist=sqrt(dx*dx+dy*dy);
        if(dist>2.0){
            m_pos.rx()+=dx/dist*m_moveSpeed;
            m_pos.ry()+=dy/dist*m_moveSpeed;
        }
    }else{
        dx=playerX-m_pos.x();
        dy=playerY-m_pos.y();
        qreal dist=sqrt(dx*dx+dy*dy);
        if(dist>GameGlobal::getSymbiosisFollowRange()){
            m_pos.rx()+=dx/dist*m_moveSpeed;
            m_pos.ry()+=dy/dist*m_moveSpeed;
        }
    }
}
bool GhostCell::isLifeExpired(qint64 nowTime)const{
    return nowTime-m_bornTime>GameGlobal::getGhostLifeTime();
}
qreal GhostCell::getX()const{
    return m_pos.x();
}
qreal GhostCell::getY()const{
    return m_pos.y();
}
int GhostCell::getSize()const{
    return m_size;
}
QColor GhostCell::getColor()const{
    return m_color;
}
bool GhostCell::checkAttackHit(const MonsterCell& mon)const{
    return CollisionUtil::circleCollide(m_pos.x(),m_pos.y(),m_size/2.0,
                                        mon.getX(),mon.getY(),mon.getSize()/2.0);
}
void GhostCell::setBornTime(qint64 time){
    m_bornTime=time;
}
qint64 GhostCell::getBornTime()const{
    return m_bornTime;
}