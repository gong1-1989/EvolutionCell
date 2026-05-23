#include "playercell.h"
#include "../Utils/randomutil.h"
PlayerCell::PlayerCell()
    :m_x(0),m_y(0)
    ,m_size(GameGlobal::getPlayerInitSize())
    ,m_speed(GameGlobal::getPlayerSpeed())
    ,m_hasSpeedBuff(false)
    ,m_hasSpeedDebuff(false) {}

void PlayerCell::move(bool w, bool a, bool s, bool d, int canvasW, int canvasH){
    qreal speed=GameGlobal::getPlayerSpeed();
    if(m_hasSpeedBuff)  speed*=GameGlobal::getSpeedBuffMult();
    if(m_hasSpeedDebuff) speed*=GameGlobal::getDebuffMult();
    if(w) m_y-=speed;
    if(s) m_y+=speed;
    if(a) m_x-=speed;
    if(d) m_x+=speed;
    int half=m_size/2;
    m_x=qBound((qreal)half,m_x,(qreal)canvasW-half);
    m_y=qBound((qreal)half,m_y,(qreal)canvasH-half);
}


void PlayerCell::grow(int addSize, GameGlobal::MonsterType type){
    int realAdd=addSize*getGeneGrowRatio();
    switch (type) {
    case GameGlobal::NORAMAL:
        m_size+=realAdd;
        break;
    case GameGlobal::ELITE:
        m_size+=realAdd*2;
        applySpeedBuff();
        break;
    case GameGlobal::SPECIAL:
        if(RandomUtil::randInt(0,1)==0){
            m_size+=realAdd/2;
            applySpeedDebuff();
        }else{
            m_size+=realAdd/4;
        }
        break;
    default:
        break;
    }
    if(m_size>GameGlobal::getPlayerMaxSize()) m_size=GameGlobal::getPlayerMaxSize();
}

bool PlayerCell::unlockGene(GameGlobal::GeneType type){
    for(auto&g:m_unlockGene){
        if(g.getType()==type) return false;
    }
    m_unlockGene.append(Gene(type));
    return true;
}
void PlayerCell::applySpeedBuff(){
    m_hasSpeedBuff=true;
}
qreal PlayerCell::getGeneGrowRatio()const{
    for(auto& g:m_unlockGene){
        if(g.getType()==GameGlobal::GENE_GROW_BOOST) return GameGlobal::getGeneGrowRatio();
    }
    return 1.0;
}
qreal PlayerCell::getGeneSpeedRatio()const{
    for(auto& g:m_unlockGene){
        if(g.getType()==GameGlobal::GENE_SPEED_UP) return GameGlobal::getGenespeedRatio();
    }
    return 1.0;
}
qreal PlayerCell::getGeneRangeRatio()const{
    for(auto& g:m_unlockGene){
        if(g.getType()==GameGlobal::GENE_RANGE_EXTEND) return GameGlobal::getGeneRangeRatio();
    }
    return 1.0;
}
const QList<Gene>& PlayerCell::getUnlockedGene() const{
    return m_unlockGene;
}

void PlayerCell::applySpeedDebuff(){
    m_hasSpeedDebuff=true;
}
void PlayerCell::clearTempEffect(){
    m_hasSpeedBuff=m_hasSpeedDebuff=false;
}
bool PlayerCell::hasSpeedBuff()const{
    return m_hasSpeedBuff;
}
qreal PlayerCell::getX()const{
    return m_x;
}
qreal PlayerCell::getY()const{
    return m_y;
}
int PlayerCell::getSize()const{
    return m_size;
}
void PlayerCell::setPos(qreal x, qreal y){
    m_x=x;
    m_y=y;
}