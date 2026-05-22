#ifndef MONSTERENTITY_H
#define MONSTERENTITY_H
//怪物实体类
#include "cellentity.h"
class MonsterEntity:public Cellentity
{
public:
    MonsterEntity();
    int m_hp;
};

#endif // MONSTERENTITY_H
