#include "monstercell.h"
#include "../Utils/randomutil.h"
#include <cmath>

MonsterCell::MonsterCell():m_x(0),m_y(0),m_size(0),m_vx(0),m_vy(0),m_type(GameGlobal::NORAMAL) {}
void MonsterCell::initRandom(qreal limitW, qreal limitH, qreal playerX, qreal playerY){
    int offset=GameGlobal::getBuffDuration();
    do{
        m_x=RandomUtil::randInt(offset,limitW-offset);
        m_y=RandomUtil::randInt(offset,limitH-offset);
    }while(hypot(m_x-playerX,m_y-playerY)<100);
    int rand=RandomUtil::randInt(0,100);
    int speed_range;
    if(rand<GameGlobal::getProbNoramal()){
        m_type=GameGlobal::NORAMAL;
        m_color=QColor(50,180,50);
        m_size=RandomUtil::randInt(GameGlobal::getMonMinSize(),GameGlobal::getMonMaxSize());
        speed_range=GameGlobal::getMonSpeedRange();
    }else if(rand<GameGlobal::getProbNoramal()+GameGlobal::getProbElite()){
        m_type=GameGlobal::ELITE;
        m_color=QColor(255,215,0);
        m_size=RandomUtil::randInt(GameGlobal::getEliteMinSize(),GameGlobal::getEliteMaxSize());
        speed_range=GameGlobal::getMonSpeedRange()*GameGlobal::getEliteSpeedMult();
    }else{
        m_type=GameGlobal::SPECIAL;
        m_color=QColor(138,43,226);
        m_size=RandomUtil::randInt(GameGlobal::getMonMinSize(),GameGlobal::getMonMaxSize());
        speed_range=GameGlobal::getMonSpeedRange()*GameGlobal::getSpecialSpeedMult();
    }
    m_vx=RandomUtil::randReal(-speed_range,speed_range);
    m_vy=RandomUtil::randReal(-speed_range,speed_range);
}

void MonsterCell::move(int canvasW, int canvasH){
    m_x+=m_vx;
    m_y+=m_vy;
    int half=m_size/2;
    if(m_x-half<0||m_x+half>canvasW){
        m_vx*=-1;
        m_x=qBound((qreal)half,m_x,(qreal)canvasW-half);
    }
    if(m_y-half<0||m_y+half>canvasH){
        m_vy*=-1;
        m_y=qBound((qreal)half,m_y,(qreal)canvasH-half);
    }
}
qreal MonsterCell::getX()const{
    return m_x;
}
qreal MonsterCell::getY()const{
    return m_y;
}
int MonsterCell::getSize()const{
    return m_size;
}
QColor MonsterCell::getColor()const{
    return m_color;
}
GameGlobal::MonsterType MonsterCell::getType()const{
    return m_type;
}