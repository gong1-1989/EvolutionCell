#ifndef LEVELSYSTEM_H
#define LEVELSYSTEM_H
#include "../Cell/cellentity.h"

class LevelSystem
{
public:
    LevelSystem();
    bool addExp(int exp);
    int getLevel()const;
    int getCurExp()const;
    int getNeedExp()const;
    void upgradeAttr(Cellentity& cell);
private:
    int m_level;
    int m_curExp;
    int m_needExp;
    const float EXP_GROW=1.2f;
};

#endif // LEVELSYSTEM_H
