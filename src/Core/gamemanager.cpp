#include "GameManager.h"

GameManager& GameManager::getInstance()
{
    static GameManager ins;
    return ins;
}

GameManager::GameManager(QObject *parent)
    : QObject(parent)
    , m_gameState(GameGlobal::RUNNING)
    , m_sceneInited(false)
    , m_eatCount(0)
    , m_normalEatNum(0)
    , m_eliteEatNum(0)
    , m_specialEatNum(0)
{
    m_monsterList.clear();
    m_ghostList.clear();
}

// ===================== 场景初始化 =====================
void GameManager::initScene(int canvasW, int canvasH)
{
    if (m_sceneInited) return;
    m_sceneInited = true;

    // 初始生成怪物
    int maxMon = GameGlobal::getMaxMonsterCount();
    for (int i = 0; i < maxMon; ++i)
        spawnMonster(canvasW, canvasH);

    qDebug() << "游戏场景初始化完成";
}

// ===================== 主帧更新（入口，仅调度原子函数） =====================
void GameManager::frameUpdate(bool keyW, bool keyA, bool keyS, bool keyD, int canvasW, int canvasH)
{
    if (!m_sceneInited || m_gameState == GameGlobal::PAUSED)
        return;

    updatePlayerEntity(keyW, keyA, keyS, keyD, canvasW, canvasH);
    updateSymbiosisSystem();
    updateGhostSystem();
    updateMonsterEntity(canvasW, canvasH);
    updateMimicSystem();
    updateRuleCheck(canvasW, canvasH);
}

// ===================== 原子更新函数：玩家 =====================
void GameManager::updatePlayerEntity(bool keyW, bool keyA, bool keyS, bool keyD, int canvasW, int canvasH)
{
    m_player.move(keyW, keyA, keyS, keyD, canvasW, canvasH);
    m_player.updateSymbiosisFollow(m_player.getX(), m_player.getY());
}

// ===================== 原子更新函数：怪物 =====================
void GameManager::updateMonsterEntity(int canvasW, int canvasH)
{
    updateMonsterAI(canvasW,canvasH);
}

// ===================== 原子更新函数：怪物AI =====================
void GameManager::updateMonsterAI(int canvasW, int canvasH){
    qreal pX=m_player.getX();
    qreal pY=m_player.getY();
    auto mimicState=m_player.getMimicState();
    for(MonsterCell& mon:m_monsterList){
        //1.计算怪物与玩家距离
        qreal dx=mon.getX()-pX;
        qreal dy=mon.getY()-pY;
        qreal dist=sqrt(dx*dx+dy*dy);
        bool inDetectRange=(dist<=mon.getDetectRange());
        //2.拟态核心:玩家伪装👉怪物无法发现
        bool canBeDetected=true;
        //虚空拟态：完全隐身
        if(mimicState==GameGlobal::MIMIC_VOID) canBeDetected=false;
        //怪物拟态：50%概率不被发现
        else if(mimicState==GameGlobal::MIMIC_MONSTER) canBeDetected=(RandomUtil::randInt(1,100)>50);
        //环境拟态：30%概率不被发现
        else if(mimicState==GameGlobal::MIMIC_ENV) canBeDetected=(RandomUtil::randInt(1,100)>30);
        //3.状态切换：在感知范围内+被发现👉追击
        if(inDetectRange&&canBeDetected) mon.setChase(true);
        else mon.setChase(false);
        mon.move(canvasW,canvasH,pX,pY);
    }
}

// ===================== 原子更新函数：共生体系统 =====================
void GameManager::updateSymbiosisSystem()
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    m_player.clearExpiredSymbiosis(now);
    checkSymbiosisAttack(0, 0);
}

// ===================== 原子更新函数：残影系统 =====================
void GameManager::updateGhostSystem()
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    // 倒序遍历，删除过期残影
    for (int i = m_ghostList.size() - 1; i >= 0; --i)
    {
        GhostCell& ghost = m_ghostList[i];
        if (ghost.isLifeExpired(now))
        {
            m_ghostList.removeAt(i);
            continue;
        }
        // 残影移动+攻击判定
        ghost.update(m_player.getX(), m_player.getY(), m_monsterList);
        for (int j = m_monsterList.size() - 1; j >= 0; --j)
        {
            MonsterCell& mon = m_monsterList[j];
            if (ghost.checkAttackHit(mon))
            {
                m_player.grow(mon.getSize() / GameGlobal::getGrowRatio(), mon.getType());
                m_eatCount++;
                if (mon.getType() == GameGlobal::NORMAL) m_normalEatNum++;
                if (mon.getType() == GameGlobal::ELITE) m_eliteEatNum++;
                if (mon.getType() == GameGlobal::SPECIAL) m_specialEatNum++;
                ghost.setBornTime(now);
                m_monsterList.removeAt(j);
                break;
            }
        }
    }
}

// ===================== 原子更新函数：通用规则（吞噬、补怪） =====================
void GameManager::updateRuleCheck(int canvasW, int canvasH)
{
    checkEat(canvasW, canvasH);
    // 怪物数量不足则补充
    int halfMax = GameGlobal::getMaxMonsterCount() / 2;
    if (m_monsterList.size() < halfMax)
        spawnMonster(canvasW, canvasH);
}

// ===================== 共生体攻击判定 =====================
void GameManager::checkSymbiosisAttack(int canvasW, int canvasH)
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    auto& symList = const_cast<QList<SymbiosisCell>&>(m_player.getSymbiosisList());

    for (int i = m_monsterList.size() - 1; i >= 0; --i)
    {
        MonsterCell& mon = m_monsterList[i];
        bool beKilled = false;

        for (auto& sym : symList)
        {
            if (!sym.canAttack(now)) continue;
            qreal attackRange=GameGlobal::getSymAttackRange();
            bool hit = CollisionUtil::circleCollision(
                sym.getX(), sym.getY(), attackRange,
                mon.getX(), mon.getY(), mon.getSize() / 2.0
                );
            if (hit)
            {
                sym.onAttackTriggered(now);
                beKilled = true;
                break;
            }
        }

        if (!beKilled) continue;

        // 吞噬收益
        m_player.grow(mon.getSize() / GameGlobal::getGrowRatio(), mon.getType());
        m_eatCount++;
        if (mon.getType() == GameGlobal::NORMAL) m_normalEatNum++;
        if (mon.getType() == GameGlobal::ELITE) m_eliteEatNum++;
        if (mon.getType() == GameGlobal::SPECIAL) m_specialEatNum++;

        // 概率生成新共生体
        int rand = RandomUtil::randInt(1, 100);
        GameGlobal::SymbiosisMode mode = GameGlobal::SYMBIO_TEMP;
        bool create = false;

        if (mon.getType() == GameGlobal::ELITE && rand <= 40)
        {
            mode = GameGlobal::SYMBIO_PERM;
            create = true;
        }
        else if (mon.getType() == GameGlobal::SPECIAL && rand <= 60)
        {
            mode = GameGlobal::SYMBIO_ABSORB;
            create = true;
        }
        else if (rand <= 25)
        {
            create = true;
        }

        if (create)
        {
            SymbiosisCell newSym;
            newSym.initSymbiosis(mon.getType(), mon.getX(), mon.getY());
            newSym.setSymbiosisMode(mode);
            newSym.setBornTime(now);
            m_player.addSymbiosis(newSym);
        }

        m_monsterList.removeAt(i);
    }
}

// ===================== 玩家吞噬判定 =====================
void GameManager::checkEat(int canvasW, int canvasH)
{
    qreal pX = m_player.getX();
    qreal pY = m_player.getY();
    int pSize = m_player.getSize();


    for (int i = m_monsterList.size() - 1; i >= 0; --i)
    {
        MonsterCell& mon = m_monsterList[i];
        bool hit = CollisionUtil::circleCollision(
            pX, pY, pSize / 2.0,
            mon.getX(), mon.getY(), mon.getSize() / 2.0
            );
        if (hit)
        {
            m_player.grow(mon.getSize() / GameGlobal::getGrowRatio(), mon.getType());
            m_eatCount++;
            if (mon.getType() == GameGlobal::NORMAL) m_normalEatNum++;
            if (mon.getType() == GameGlobal::ELITE) m_eliteEatNum++;
            if (mon.getType() == GameGlobal::SPECIAL) m_specialEatNum++;

            // 概率生成共生体
            int rand = RandomUtil::randInt(1, 100);
            GameGlobal::SymbiosisMode mode = GameGlobal::SYMBIO_TEMP;
            bool create = false;
            if (mon.getType() == GameGlobal::ELITE && rand <= 40) { mode = GameGlobal::SYMBIO_PERM; create = true; }
            else if (mon.getType() == GameGlobal::SPECIAL && rand <= 60) { mode = GameGlobal::SYMBIO_ABSORB; create = true; }
            else if (rand <= 25) create = true;

            if (create)
            {
                SymbiosisCell newSym;
                newSym.initSymbiosis(mon.getType(), mon.getX(), mon.getY());
                newSym.setSymbiosisMode(mode);
                newSym.setBornTime(QDateTime::currentMSecsSinceEpoch());
                m_player.addSymbiosis(newSym);
            }
            m_monsterList.removeAt(i);
        }
    }
}

// ===================== 怪物生成 =====================
void GameManager::spawnMonster(int canvasW, int canvasH)
{
    int probNormal = GameGlobal::getProbNormal();
    int probElite = GameGlobal::getProbElite();
    int rand = RandomUtil::randInt(1, 100);

    GameGlobal::MonsterType type;
    if (rand <= probNormal)
        type = GameGlobal::NORMAL;
    else if (rand <= probNormal + probElite)
        type = GameGlobal::ELITE;
    else
        type = GameGlobal::SPECIAL;

    MonsterCell mon;
    mon.initMonster(type, canvasW, canvasH);
    m_monsterList.append(mon);
}

// ===================== 游戏重置 =====================
void GameManager::resetNewGame(int canvasW, int canvasH)
{
    m_player = PlayerCell();
    m_monsterList.clear();
    m_ghostList.clear();

    m_eatCount = 0;
    m_normalEatNum = 0;
    m_eliteEatNum = 0;
    m_specialEatNum = 0;

    m_sceneInited = false;
    m_gameState = GameGlobal::RUNNING;

    // 开局记录初始演化节点
    m_player.recordCurrentEvolveNode();
    initScene(canvasW, canvasH);
}

// ===================== 游戏状态控制 =====================
GameGlobal::GameState GameManager::getGameState() const { return m_gameState; }
void GameManager::pauseGame() { m_gameState = GameGlobal::PAUSED; }
void GameManager::resumeGame() { m_gameState = GameGlobal::RUNNING; }

// ===================== 躯体解构 =====================
void GameManager::executeBodyDecompose(GameGlobal::DecomposeLevel lv)
{
    if (m_gameState != GameGlobal::RUNNING) return;
    m_player.executeDecompose(lv);
    // 解构成功自动记录节点
    m_player.recordCurrentEvolveNode();
}

// ===================== 时空回溯接口 =====================
bool GameManager::rollbackToLastNode()
{
    bool ret = m_player.rollbackToLastNode();
    if (!ret) return false;

    // 回溯成功生成残影
    GhostCell ghost;
    ghost.initGhost(m_player.getX(), m_player.getY(), m_player.getSize(), m_player.getLawType());
    ghost.setBornTime(QDateTime::currentMSecsSinceEpoch());
    m_ghostList.append(ghost);
    return true;
}

bool GameManager::rollbackToAssignNode(int index)
{
    bool ret = m_player.rollbackToAssignNode(index);
    if (!ret) return false;

    GhostCell ghost;
    ghost.initGhost(m_player.getX(), m_player.getY(), m_player.getSize(), m_player.getLawType());
    ghost.setBornTime(QDateTime::currentMSecsSinceEpoch());
    m_ghostList.append(ghost);
    return true;
}

int GameManager::getHistoryNodeTotal() const
{
    return m_player.getHistoryNodeCount();
}

// ===================== 状态查询接口 =====================
int GameManager::getPlayerSymbiosisNum() const { return m_player.getSymbiosisCount(); }
GameGlobal::RejectLevel GameManager::getPlayerRejectLevel() const { return m_player.getCurrentRejectLevel(); }
int GameManager::getGhostCount() const { return m_ghostList.size(); }
const QList<SymbiosisCell>& GameManager::getSymbiosisList() const { return m_player.getSymbiosisList(); }
const QList<GhostCell>& GameManager::getGhostList() const { return m_ghostList; }
int GameManager::getEatTotal() const { return m_eatCount; }
const PlayerCell& GameManager::getPlayer() const{return m_player;}
const QList<MonsterCell>& GameManager::getMonsterList() const{return m_monsterList;}
GameGlobal::MimicState GameManager::getPlayerMimicStable()const{return m_player.getMimicState();}
int GameManager::getPlayerGeneStable()const{return m_player.getGeneStableValue();}
GameGlobal::GeneStableState GameManager::getGeneStableState()const{return m_player.getGeneStableState();}

// ===================== 从存档加载游戏 =====================
bool GameManager::loadGameFromFile(const QString &filePath){
    //1.调用SaveManager读取文件内容
    QJsonObject loadData=SaveManager::getInstance().loadSave(filePath);
    if(loadData.isEmpty()){
        qDebug()<<"存档文件读取失败："<<filePath;
    }
    //2.加载存档不需要初始化场景
    m_sceneInited = true;
    //3.恢复玩家数据
    if(loadData.contains("player")){
        m_player.fromJson(loadData.value("player").toObject());
        qDebug()<<"玩家数据恢复完成";
    }
    //4.恢复共生体列表
    if(loadData.contains("symbiosis_list")){
        QJsonArray symArr=loadData.value("symbiosis_list").toArray();
        for(const auto& symVal :symArr){
            SymbiosisCell cell;
            cell.fromJson(symVal.toObject());
            m_player.addSymbiosis(cell);
        }
        qDebug()<<"共生体恢复完成，数量："<<m_player.getSymbiosisList().size();
    }
    //5.恢复时空残影列表
    if(loadData.contains("ghost_list")){
        QJsonArray ghostArr=loadData.value("ghost_list").toArray();
        for(const auto& ghostVal :ghostArr){
            GhostCell cell;
            cell.fromJson(ghostVal.toObject());
            m_ghostList.append(cell);
        }
        qDebug()<<"时空残影恢复完成，数量："<<m_ghostList.size();
    }
    //6.恢复怪物列表
    if(loadData.contains("monster_list")){
        QJsonArray monsterArr=loadData.value("monster_list").toArray();
        for(const auto& monVal:monsterArr){
            MonsterCell cell;
            cell.fromJson(monVal.toObject());
            m_monsterList.append(cell);
        }
    }
    //7.恢复全局吞噬计数
    if(loadData.contains("eat_total")) m_eatCount=loadData.value("eat_total").toInt();
    if(loadData.contains("eat_normal")) m_normalEatNum=loadData.value("eat_normal").toInt();
    if(loadData.contains("eat_elite")) m_eliteEatNum=loadData.value("eat_elite").toInt();
    if(loadData.contains("eat_special")) m_specialEatNum=loadData.value("eat_special").toInt();
    qDebug()<<"✔存档加载成功："<<filePath;
    return true;
}
// ====================== 快速存档 ======================
void GameManager::quickSaveGame(){
    // 组装全量存档数据
    QJsonObject rootData;
    rootData["player"] = m_player.toJson();

    QJsonArray symArr;
    for (const SymbiosisCell& sym : m_player.getSymbiosisList())
        symArr.append(sym.toJson());
    rootData["symbiosis_list"] = symArr;

    QJsonArray ghostArr;
    for (const GhostCell& ghost : m_ghostList)
        ghostArr.append(ghost.toJson());
    rootData["ghost_list"] = ghostArr;

    QJsonArray monsterArr;
    for (const MonsterCell& mon:m_monsterList)
        monsterArr.append(mon.toJson());
    rootData["monster_list"]=monsterArr;

    rootData["eat_total"]=m_eatCount;
    rootData["eat_normal"]=m_normalEatNum;
    rootData["eat_elite"]=m_eliteEatNum;
    rootData["eat_special"]=m_specialEatNum;
    SaveManager::getInstance().createNewSave(rootData);
}
//======================== 拟态、熵变系统 ================
void GameManager::updateMimicSystem(){
    qint64 now=QDateTime::currentMSecsSinceEpoch();
    //更新玩家状态
    m_player.updateMimic(now);
}
void GameManager::startPlayerMimic(GameGlobal::MimicState type,qint64 duration){
    m_player.startMimic(type,duration);
}

void GameManager::triggerPlayerGeneChaos(){
    m_player.triggerGeneChao();
}