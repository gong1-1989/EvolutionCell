#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
//核心管理器
#include <QList>
#include <QDateTime>
#include "gameglobal.h"
#include "Cell/playercell.h"
#include "Cell/monstercell.h"
#include "Utils/collisionutil.h"
#include "Utils/savemanager.h"
#include "Utils/randomutil.h"
class GameManager
{
public:
    GameManager();
    void initScene(int w,int h);
    void frameUpdate(bool keyW,bool keyA,bool keyS,bool keyD,int canvasW,int canvasH);

    void pauseGame();
    void resumeGame();
    GameGlobal::GameState getGameState()const;
    int getEacCount()const;

    const PlayerCell& getPlayer()const;
    const QList<MonsterCell>& getMonsterList()const;

    bool loadFromSaveSlot(const QString& path);
    QString saveToSaveSlot();
    void reserNewGame();

    void initPlayerLifeLaw(GameGlobal::LifeLaw law);
    bool executeBodyDecompose(GameGlobal::DecomposeLevel level);
    GameGlobal::LifeLaw getPlayerLawType()const;
    GameGlobal::DecomposeLevel getplayerDecomposeLv()const;
    int getPlayerDecomposeRisk()const;

    int getPlayerSymbiosisNum()const;
    GameGlobal::RejectLevel getPlayerRejectLevel()const;
    const QList<SymbiosisCell>& getSymbiosisList()const;
private:
    void spawnMonster(int canvasW, int canvasH);
    void checkEat(int canvasW,int canvasH);
    void updateBuffStatus();
    PlayerCell m_player;
    QList<MonsterCell> m_monsterList;
    bool m_sceneInited;
    qint64 m_buffStartTime;
    bool m_buffActive;

    GameGlobal::GameState m_gameState;
    int m_eatCount;

    int m_normalEatNum;
    int m_eliteEatNum;
    int m_specialEatNum;

    void updateSymbiosisSystem();
    void checkSymbiosisAttack(int canvasW, int canvasH);
};

#endif // GAMEMANAGER_H
