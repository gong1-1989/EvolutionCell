#include "monsterentity.h"

MonsterEntity::MonsterEntity() {
    m_pos=QPointF(200,200);
    m_radius=15;
    m_moveSpeed=70.0;
    m_cellcolor=QColor(60,220,80);
    m_hp=35;
    m_attack=6;
    m_expDrop=15;
}
int MonsterEntity::checkDead(QList<MonsterEntity> &monList){
    int totalExp=0;
    QList<MonsterEntity> aliveList;
    for(MonsterEntity& mon:monList){
        if(mon.m_hp>0) aliveList.append(mon);
        else totalExp+=mon.m_expDrop;
    }
    monList.swap(aliveList);
    return totalExp;
}