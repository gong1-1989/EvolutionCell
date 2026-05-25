#include "symbiosiscell.h"
#include <cmath>

SymbiosisCell::SymbiosisCell():m_mode(GameGlobal::SYMBIO_PERM)
    ,m_sourceType(GameGlobal::NORAMAL)
    ,m_size(10),m_bornTime(0),m_lastAttackTime(0),m_moveSpeed(1.8)
{}
void SymbiosisCell::initSymbiosis(GameGlobal::MonsterType monType, qreal x, qreal y){
    m_sourceType=monType;
    m_pos.setX(x);
    m_pos.setY(y);
    switch(monType){
    case GameGlobal::NORAMAL:
        m_size=10;
        m_color=QColor(100,200,100);
        break;
    case GameGlobal::ELITE:
        m_size=14;
        m_color=QColor(255,220,60);
        break;
    case GameGlobal::SPECIAL:
        m_size=12;
        m_color=QColor(150,60,230);
        break;
    }
}
void SymbiosisCell::followUpdate(qreal playerX, qreal playerY, qreal followRange, const QList<MonsterCell> &monsterList){
    const MonsterCell* nearestMon=nullptr;
    qreal minDist=999999.0;
    for(const auto& mon:monsterList){
        qreal dx=mon.getX()-m_pos.x();
        qreal dy=mon.getY()-m_pos.y();
        qreal dist=sqrt(dx*dx+dy*dy);
        if(dist<minDist){
            minDist=dist;
            nearestMon=&mon;
        }
    }
    bool foundTaget=false;
    if(nearestMon){
        qreal dx=nearestMon->getX()-m_pos.x();
        qreal dy=nearestMon->getY()-m_pos.y();
        qreal dist=sqrt(dx*dx+dy*dy);
        if(dist>0.1){
            m_pos.rx()+=dx/dist*m_moveSpeed;
            m_pos.ry()+=dy/dist*m_moveSpeed;
            foundTaget=true;
        }
    }
    if(!foundTaget){
        qreal dx=playerX-m_pos.x();
        qreal dy=playerY-m_pos.y();
        qreal dist=sqrt(dx*dx+dy*dy);
        if(dist>followRange){
            m_pos.rx()+=dx/dist*m_moveSpeed;
            m_pos.ry()+=dy/dist*m_moveSpeed;
        }
    }
}
bool SymbiosisCell::canAttack(qint64 currentTime) const{
    int cd=GameGlobal::getSymAttackCdMs();
    return (currentTime-m_lastAttackTime)>=cd;
}
void SymbiosisCell::onAttackTriggered(qint64 currentTime){
    m_lastAttackTime=currentTime;
}
bool SymbiosisCell::isTempExpired(qint64 currentTime) const{
    //非临时寄生可直接判定未过期
    if(m_mode!=GameGlobal::SYMBIO_TEMP)return false;
    int duration=GameGlobal::getTempSymbiosisDuration();
    return currentTime-m_bornTime>duration;
}
qreal SymbiosisCell::getX() const{
    return m_pos.x();
}
qreal SymbiosisCell::getY() const{
    return m_pos.y();
}
int SymbiosisCell::getSize()const{
    return m_size;
}
QColor SymbiosisCell::getColor()const{
    return m_color;
}
GameGlobal::SymbiosisMode SymbiosisCell::getMode()const{
    return m_mode;
}
GameGlobal::MonsterType SymbiosisCell::getSourceType()const{
    return m_sourceType;
}
void SymbiosisCell::setSymbiosisMode(GameGlobal::SymbiosisMode mode){
    m_mode=mode;
}
void SymbiosisCell::setBornTime(qint64 time){
    m_bornTime=time;
}
qint64 SymbiosisCell::getBornTime()const{
    return m_bornTime;
}
void SymbiosisCell::setLastAttackTime(qint64 time){
    m_lastAttackTime=time;
}
qint64 SymbiosisCell::getLastAttackTime()const{
    return m_lastAttackTime;
}