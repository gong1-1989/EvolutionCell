#include "levelsystem.h"

LevelSystem::LevelSystem() {
    m_level=1;
    m_curExp=0;
    m_needExp=100;
}
bool LevelSystem::addExp(int exp){
    bool isUpgrade=false;
    m_curExp+=exp;
    while(m_curExp>=m_needExp){
        m_curExp-=m_needExp;
        m_level++;
        m_needExp=qRound(m_needExp*EXP_GROW);
        isUpgrade=true;
    }
    return isUpgrade;
}

int LevelSystem::getLevel(){
    return m_level;
}
int LevelSystem::getCurExp(){
    return m_curExp;
}
int LevelSystem::getNeedExp(){
    return m_needExp;
}
void LevelSystem::upgradeAttr(Cellentity &cell){
    cell.m_hp+=15;
    cell.m_attack+=2;
}