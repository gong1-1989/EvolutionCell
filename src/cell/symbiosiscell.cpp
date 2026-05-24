#include "symbiosiscell.h"
#include <cmath>

SymbiosisCell::SymbiosisCell():m_mode(GameGlobal::SYMBIO_PERM)
    ,m_sourceType(GameGlobal::NORAMAL)
    ,m_size(10),m_bornTime(0)
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
void SymbiosisCell::followUpdate(qreal playerX, qreal playerY, qreal followRange){
    //计算与本体距离，保持跟随间距
    qreal dx=playerX-m_pos.x();
    qreal dy=playerY-m_pos.y();
    qreal dist=sqrt(dx*dx+dy*dy);
    if(dist>followRange){
        qreal moveStep=1.2;
        m_pos.rx()+=dx/dist*moveStep;
        m_pos.ry()+=dy/dist*moveStep;
    }
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
void SymbiosisCell::setSymbiosisMode(GameGlobal::SymbiosisMode mode){
    m_mode=mode;
}
void SymbiosisCell::setBornTime(qint64 time){
    m_bornTime=time;
}