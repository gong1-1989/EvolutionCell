#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "GameGlobal.h"
#include "Cell/PlayerCell.h"
#include "Cell/monstercell.h"
#include "Cell/SymbiosisCell.h"
#include "Cell/GhostCell.h"
#include "Utils/savemanager.h"
#include "Utils/RandomUtil.h"
#include "Utils/CollisionUtil.h"
#include <QDateTime>
#include <QDebug>
#include <QObject>
#include <QList>

/**
 * @brief 游戏全局管理器（单例）
 * 职责：帧调度、实体管理、规则判定、按键逻辑转发、游戏重置
 * 优化：拆分臃肿帧循环，按模块拆分为原子更新函数
 */
class GameManager : public QObject
{
    Q_OBJECT
public:
    // 单例
    static GameManager& getInstance();
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    /**
     * @brief 初始化游戏场景
     */
    void initScene(int canvasW, int canvasH);

    /**
     * @brief 游戏主帧更新（入口）
     */
    void frameUpdate(bool keyW, bool keyA, bool keyS, bool keyD, int canvasW, int canvasH);

    /**
     * @brief 重置为新游戏
     */
    void resetNewGame(int canvasW, int canvasH);

    /**
     * @brief 从存档文件加载游戏
     */
    bool loadGameFromFile(const QString &filePath);

    /**
     * @brief 快速存档
     */
    void quickSaveGame();

    // 游戏状态控制
    GameGlobal::GameState getGameState() const;
    void pauseGame();
    void resumeGame();

    // 躯体解构触发
    void executeBodyDecompose(GameGlobal::DecomposeLevel lv);

    // 时空回溯对外接口
    bool rollbackToLastNode();
    bool rollbackToAssignNode(int index);
    int getHistoryNodeTotal() const;

    // 状态查询（HUD/渲染使用）
    int getPlayerSymbiosisNum() const;
    GameGlobal::RejectLevel getPlayerRejectLevel() const;
    int getGhostCount() const;
    const QList<SymbiosisCell>& getSymbiosisList() const;
    const QList<GhostCell>& getGhostList() const;
    // 获取玩家实例（只读）
    const PlayerCell& getPlayer() const;
    // 获取怪物列表（只读）
    const QList<MonsterCell>& getMonsterList() const;
    //拟态、熵变 查询接口
    GameGlobal::MimicState getPlayerMimicStable()const;
    int getPlayerGeneStable()const;
    GameGlobal::GeneStableState getGeneStableState()const;

    // 怪物生成
    void spawnMonster(int canvasW, int canvasH);

    // 统计数据
    int getEatTotal() const;

    //启动玩家状态
    void startPlayerMimic(GameGlobal::MimicState type,qint64 duration);
    //触发玩家基因熵变
    void triggerPlayerGeneChaos();

private:
    explicit GameManager(QObject *parent = nullptr);

    // ===================== 帧循环拆分：原子更新函数 =====================
    void updatePlayerEntity(bool keyW, bool keyA, bool keyS, bool keyD, int canvasW, int canvasH);
    void updateMonsterEntity(int canvasW, int canvasH);
    void updateMonsterAI(int canvasW,int canvasH);
    void updateSymbiosisSystem();
    void updateGhostSystem();
    void updateRuleCheck(int canvasW, int canvasH);

    /**
     * @brief 拟态系统更新
     */
    void updateMimicSystem();

    // 业务规则
    void checkEat(int canvasW, int canvasH);
    void checkSymbiosisAttack(int canvasW, int canvasH);

private:
    // 全局状态
    GameGlobal::GameState m_gameState;
    bool m_sceneInited;

    // 实体容器
    PlayerCell m_player;
    QList<MonsterCell> m_monsterList;
    QList<GhostCell> m_ghostList;

    // 全局统计
    int m_eatCount;
    int m_normalEatNum;
    int m_eliteEatNum;
    int m_specialEatNum;
};

#endif // GAMEMANAGER_H