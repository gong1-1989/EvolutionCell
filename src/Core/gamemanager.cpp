#include "gamemanager.h"

GameManager::GameManager():m_sceneInited(false)
    ,m_buffActive(false)
    ,m_gameState(GameGlobal::RUNING)
    ,m_eatCount(0)
    ,m_normalEatNum(0)
    ,m_eliteEatNum(0)
    ,m_specialEatNum(0){
    m_ghostList.clear();
}

void GameManager::initScene(int w, int h){
    m_player.setPos(w/2.0,h/2.0);
    m_monsterList.clear();
    for(int i=0;i<GameGlobal::getMaxMonsterCount();i++){
        spawnMonster(w,h);
    }
    m_sceneInited=true;
}

void GameManager::pauseGame(){
    m_gameState=GameGlobal::PAUSED;
}
void GameManager::resumeGame(){
    m_gameState=GameGlobal::RUNING;
}
GameGlobal::GameState GameManager::getGameState()const{
    return m_gameState;
}
int GameManager::getEacCount()const{
    return m_eatCount;
}
void GameManager::frameUpdate(bool keyW, bool keyA, bool keyS, bool keyD, int canvasW, int canvasH){
    if(!m_sceneInited||m_gameState==GameGlobal::PAUSED)return;
    updateBuffStatus();
    m_player.move(keyW,keyA,keyS,keyD,canvasW,canvasH);

    qreal followRange=GameGlobal::getSymbiosisFollowRange();

    QList<SymbiosisCell>&symList=m_player.getMutableSymbiosisList();
    for(SymbiosisCell& sym:symList){
        sym.followUpdate(m_player.getX(),m_player.getY(),followRange,m_monsterList);
    }
    checkSymbiosisAttack(canvasW,canvasH);

    updateGhostSystem(canvasW,canvasH);

    for(MonsterCell &mon:m_monsterList){
        mon.move(canvasW,canvasH);
    }
    checkEat(canvasW,canvasH);
    updateSymbiosisSystem();
}
void GameManager::checkSymbiosisAttack(int canvasW, int canvasH){
    qint64 now=QDateTime::currentMSecsSinceEpoch();
    QList<SymbiosisCell>& symList=m_player.getMutableSymbiosisList();
    for(int i=m_monsterList.size()-1;i>=0;--i){
        auto&mon=m_monsterList[i];
        bool beKilled=false;
        for(SymbiosisCell& sym:symList){
            if(!sym.canAttack(now)) continue;
            if(CollisionUtil::circleCollide(sym.getX(),sym.getY(),sym.getSize()/2+5.0,
                                             mon.getX(),mon.getY(),mon.getSize()/2+5.0)){
                sym.onAttackTriggered(now);
                beKilled=true;
                break;
            }
        }
        if(beKilled){
            m_player.grow(mon.getSize()/GameGlobal::getGrowRatio(),mon.getType());
            m_eatCount++;
            if(mon.getType()==GameGlobal::NORAMAL) m_normalEatNum++;
            if(mon.getType()==GameGlobal::ELITE) {
                m_eliteEatNum++;
                m_buffStartTime=QDateTime::currentMSecsSinceEpoch();
                m_buffActive=true;
            }
            if(mon.getType()==GameGlobal::SPECIAL) m_specialEatNum++;
            if(m_normalEatNum>=GameGlobal::getUnlockRangeNum()) m_player.unlockGene(GameGlobal::GENE_RANGE_EXTEND);
            if(m_eliteEatNum>=GameGlobal::getUnlockSpeedNum()) m_player.unlockGene(GameGlobal::GENE_SPEED_UP);
            if(m_specialEatNum>=GameGlobal::getUnlockGrowNum()) m_player.unlockGene(GameGlobal::GENE_GROW_BOOST);
            m_monsterList.removeAt(i);
            if(m_monsterList.size()<GameGlobal::getMaxMonsterCount()/2){
                for(int j=m_monsterList.size();j<GameGlobal::getMaxMonsterCount();j++){
                    spawnMonster(canvasW,canvasH);
                }
            }

            int randRate=RandomUtil::randInt(1,100);
            GameGlobal::SymbiosisMode createMode;
            if(mon.getType()==GameGlobal::ELITE&&randRate<=40)createMode=GameGlobal::SYMBIO_PERM;
            else if(mon.getType()==GameGlobal::SPECIAL&&randRate<=60)createMode=GameGlobal::SYMBIO_ABSORB;
            else if(randRate<=25)createMode=GameGlobal::SYMBIO_TEMP;
            else return;
            SymbiosisCell newSymCell;
            newSymCell.initSymbiosis(mon.getType(),mon.getX(),mon.getY());
            newSymCell.setSymbiosisMode(createMode);
            newSymCell.setBornTime(QDateTime::currentMSecsSinceEpoch());
            m_player.addSymbiosisCell(newSymCell);
        }
    }
}
void GameManager::updateBuffStatus(){
    if(m_buffActive){
        qint64 current=QDateTime::currentMSecsSinceEpoch();
        if(current-m_buffStartTime>GameGlobal::getBuffDuration()){
            m_player.clearTempEffect();
            m_buffActive=false;
        }
    }
}
void GameManager::spawnMonster(int canvasW, int canvasH){
    MonsterCell mon;
    mon.initRandom(canvasW,canvasH,m_player.getX(),m_player.getY());
    m_monsterList.append(mon);
}
void GameManager::checkEat(int canvasW, int canvasH){
    int pSize=m_player.getSize();
    qreal pX=m_player.getX();
    qreal pY=m_player.getY();
    qreal rangeRate=m_player.getGeneRangeRatio();
    for(int i=m_monsterList.size()-1;i>=0;i--){
        MonsterCell & mon=m_monsterList[i];
        if(CollisionUtil::circleCollide(pX,pY,pSize/2*rangeRate,mon.getX(),mon.getY(),mon.getSize()/2)){
            m_player.grow(mon.getSize()/GameGlobal::getGrowRatio(),mon.getType());
            m_eatCount++;
            if(mon.getType()==GameGlobal::NORAMAL) m_normalEatNum++;
            if(mon.getType()==GameGlobal::ELITE) {
                m_eliteEatNum++;
                m_buffStartTime=QDateTime::currentMSecsSinceEpoch();
                m_buffActive=true;
            }
            if(mon.getType()==GameGlobal::SPECIAL) m_specialEatNum++;
            if(m_normalEatNum>=GameGlobal::getUnlockRangeNum()) m_player.unlockGene(GameGlobal::GENE_RANGE_EXTEND);
            if(m_eliteEatNum>=GameGlobal::getUnlockSpeedNum()) m_player.unlockGene(GameGlobal::GENE_SPEED_UP);
            if(m_specialEatNum>=GameGlobal::getUnlockGrowNum()) m_player.unlockGene(GameGlobal::GENE_GROW_BOOST);
            m_monsterList.removeAt(i);
            if(m_monsterList.size()<GameGlobal::getMaxMonsterCount()/2){
                for(int j=m_monsterList.size();j<GameGlobal::getMaxMonsterCount();j++){
                    spawnMonster(canvasW,canvasH);
                }
            }

            int randRate=RandomUtil::randInt(1,100);
            GameGlobal::SymbiosisMode createMode;
            if(mon.getType()==GameGlobal::ELITE&&randRate<=40)createMode=GameGlobal::SYMBIO_PERM;
            else if(mon.getType()==GameGlobal::SPECIAL&&randRate<=60)createMode=GameGlobal::SYMBIO_ABSORB;
            else if(randRate<=25)createMode=GameGlobal::SYMBIO_TEMP;
            else return;
            SymbiosisCell newSymCell;
            newSymCell.initSymbiosis(mon.getType(),mon.getX(),mon.getY());
            newSymCell.setSymbiosisMode(createMode);
            newSymCell.setBornTime(QDateTime::currentMSecsSinceEpoch());
            m_player.addSymbiosisCell(newSymCell);
        }
    }
}
const PlayerCell& GameManager::getPlayer() const{
    return m_player;
}
const QList<MonsterCell>& GameManager::getMonsterList() const{
    return m_monsterList;
}

QString GameManager::saveToSaveSlot(){
    QJsonObject data;
    QJsonObject playerObj;
    playerObj["x"]=m_player.getX();
    playerObj["y"]=m_player.getY();
    playerObj["size"]=m_player.getSize();
    data["player"]=playerObj;
    QJsonArray geneArr;
    const auto& geneList=m_player.getUnlockedGene();
    for(const auto& g:geneList){
        geneArr.append(g.getType());
    }
    data["gene"]=geneArr;
    QJsonObject statObj;
    statObj["total_eat"]=m_eatCount;
    statObj["normal"]=m_normalEatNum;
    statObj["elite"]=m_eliteEatNum;
    statObj["special"]=m_specialEatNum;
    data["stat"]=statObj;
    QJsonArray symbiosisArr;
    const auto& symList=m_player.getSymbiosisList();
    for(const auto& symCell:symList){
        QJsonObject symObj;
        symObj["mode"]=(int)symCell.getMode();
        symObj["source_type"]=(int)symCell.getSourceType();
        symObj["pos_x"]=symCell.getX();
        symObj["pos_y"]=symCell.getY();
        symObj["born_time"]=symCell.getBornTime();
        symObj["last_attack_time"]=symCell.getLastAttackTime();
        symbiosisArr.append(symObj);
    }
    data["symbiosis_list"]=symbiosisArr;
    data["gen_reject_value"]=m_player.getGeneRejectValue();

    QJsonArray historyNodeArr;
    int nodeCount=m_player.getHistoryNodeCount();
    for(int i=0;i<nodeCount;++i){
        GameGlobal::EvolveHistoryNode node=m_player.getHistoryNodeByiIndex(i);
        QJsonObject nodeObj;
        nodeObj["pos_x"]=node.posX;
        nodeObj["pos_y"]=node.posY;
        nodeObj["body_size"]=node.bodySize;
        nodeObj["law_type"]=(int)node.currentLaw;
        nodeObj["decompose_lv"]=(int)node.decomposeLv;
        nodeObj["decompose_risk"]=node.decomposeRisk;
        nodeObj["gene_reject"]=node.geneRejectValue;
        QJsonArray geneTypeArr;
        for(int t:node.unlockGeneType){
            geneTypeArr.append(t);
        }
        nodeObj["gene_list"]=geneTypeArr;
        historyNodeArr.append(nodeObj);
    }
    data["history_node_list"]=historyNodeArr;

    QJsonArray ghostArr;
    for(const GhostCell& ghost:m_ghostList){
        QJsonObject ghostObj;
        ghostObj["pos_x"]=ghost.getX();
        ghostObj["pos_y"]=ghost.getY();
        ghostObj["size"]=ghost.getSize();
        ghostObj["born_time"]=ghost.getBornTime();
        ghostArr.append(ghostObj);
    }
    data["ghost_list"]=ghostArr;
    return SaveManager::getInstance().saveToSlot(data);
}
bool GameManager::loadFromSaveSlot(const QString &path){
    QJsonObject data=SaveManager::getInstance().loadFromSlot(path);
    if(data.isEmpty()) return false;
    QJsonObject pObj=data["player"].toObject();
    qreal px=pObj["x"].toDouble();
    qreal py=pObj["y"].toDouble();
    int pSize=pObj["size"].toInt();
    m_player.setPos(px,py);
    int initS=GameGlobal::getPlayerInitSize();
    int add=pSize-initS;
    if(add>0)m_player.grow(add,GameGlobal::NORAMAL);
    QJsonArray geneArr=data["gene"].toArray();
    for(auto val:geneArr){
        GameGlobal::GeneType t=static_cast<GameGlobal::GeneType>(val.toInt());
        m_player.unlockGene(t);
    }
    QJsonObject stObj=data["stat"].toObject();
    m_eatCount=stObj["total_eat"].toInt();
    m_normalEatNum=stObj["normal"].toInt();
    m_eliteEatNum=stObj["elite"].toInt();
    m_specialEatNum=stObj["special"].toInt();
    m_player.resetSymbiosis();
    m_player.setGenRejectValue(data.value("gene_reject_value").toInt(0));
    QJsonArray symbiosisArr=data.value("symbiosis_list").toArray();
    for(auto symVal:symbiosisArr){
        QJsonObject symObj=symVal.toObject();
        SymbiosisCell symCell;
        int modeInt=symObj.value("mode").toInt();
        int typeInt=symObj.value("source_type").toInt();
        qreal x=symObj.value("pos_x").toDouble();
        qreal y=symObj.value("pos_y").toDouble();
        qint64 bornTime=symObj.value("born_time").toVariant().toLongLong();
        qint64 lastAtkTime=symObj.value("last_attack_time").toVariant().toLongLong();
        GameGlobal::SymbiosisMode mod=static_cast<GameGlobal::SymbiosisMode>(modeInt);
        GameGlobal::MonsterType srcType=static_cast<GameGlobal::MonsterType>(typeInt);
        symCell.initSymbiosis(srcType,x,y);
        symCell.setSymbiosisMode(mod);
        symCell.setBornTime(bornTime);
        symCell.setLastAttackTime(lastAtkTime);
        m_player.addSymbiosisCell(symCell);
    }
    m_player.resetHistory();
    QJsonArray historyNodeArr=data.value("history_node_list").toArray();
    for(auto nodeVal:historyNodeArr){
        QJsonObject nodeObj=nodeVal.toObject();
        GameGlobal::EvolveHistoryNode node;
        node.posX=nodeObj.value("pos_x").toDouble();
        node.posY=nodeObj.value("pos_y").toDouble();
        node.bodySize=nodeObj.value("body_size").toInt();
        node.currentLaw=static_cast<GameGlobal::LifeLaw>(nodeObj.value("law_type").toInt());
        node.decomposeLv=static_cast<GameGlobal::DecomposeLevel>(nodeObj.value("decompose_lv").toInt());
        node.decomposeRisk=nodeObj.value("decompose_risk").toInt();
        node.geneRejectValue=nodeObj.value("gene_reject").toInt();
        QJsonArray geneTypeArr=nodeObj.value("gene_list").toArray();
        for(auto gVal:geneTypeArr){
            node.unlockGeneType.append(gVal.toInt());
        }
        m_player.setHistoryList(node);
    }

    m_ghostList.clear();
    QJsonArray ghostArr=data.value("ghost_list").toArray();
    for(auto ghostVal:ghostArr){
        QJsonObject ghostObj=ghostVal.toObject();
        GhostCell ghost;
        qreal x=ghostObj.value("pos_x").toDouble();
        qreal y=ghostObj.value("pos_y").toDouble();
        int size=ghostObj.value("size").toInt();
        qint64 bornTime=ghostObj.value("born_time").toVariant().toLongLong();
        ghost.initGhost(x,y,size,m_player.getCurrentLaw());
        ghost.setBornTime(bornTime);
        m_ghostList.append(ghost);
    }
    return true;
}
void GameManager::reserNewGame(){
    m_player=PlayerCell();
    m_monsterList.clear();
    m_eatCount=0;
    m_normalEatNum=0;
    m_eliteEatNum=0;
    m_specialEatNum=0;
    m_sceneInited=false;
    m_gameState=GameGlobal::RUNING;
    m_player.resetSymbiosis();
    m_ghostList.clear();
    m_player.recordCurrentEvolveNode();
}
int GameManager::getPlayerDecomposeRisk()const{
    return m_player.getDecomposeRisk();
}
void GameManager::initPlayerLifeLaw(GameGlobal::LifeLaw law){
    m_player.initLifeLaw(law);
}
bool GameManager::executeBodyDecompose(GameGlobal::DecomposeLevel level){
    return m_player.doDecompose(level);
}
GameGlobal::LifeLaw GameManager::getPlayerLawType()const{
    return m_player.getCurrentLaw();
}
GameGlobal::DecomposeLevel GameManager::getplayerDecomposeLv()const{
    return m_player.getDecomposeLevel();
}
void GameManager::updateSymbiosisSystem(){
    qint64 now=QDateTime::currentMSecsSinceEpoch();
    m_player.clealExpiredSymbiosis(now);
}
int GameManager::getPlayerSymbiosisNum()const{
    return m_player.getSymbiosisCount();
}
GameGlobal::RejectLevel GameManager::getPlayerRejectLevel()const{
    return m_player.getcurrentRejectLevel();
}
const QList<SymbiosisCell>& GameManager::getSymbiosisList() const{
    return m_player.getSymbiosisList();
}

void GameManager::updateGhostSystem(int canvasW, int canvasH){
    qint64 now=QDateTime::currentMSecsSinceEpoch();
    qreal atkMult=GameGlobal::getGhostAttackMult();
    for(int i=m_ghostList.size()-1;i>=0;--i){
        GhostCell& ghost=m_ghostList[i];
        if(ghost.isLifeExpired(now)){
            m_ghostList.removeAt(i);
            continue;
        }
        ghost.update(m_player.getX(),m_player.getY(),m_monsterList);
        for(int j=m_monsterList.size()-1;j>=0;--j){
            MonsterCell& mon=m_monsterList[j];
            if(ghost.checkAttackHit(mon)){
                m_player.grow(mon.getSize()/GameGlobal::getGrowRatio(),mon.getType());
                m_eatCount++;
                if(mon.getType()==GameGlobal::NORAMAL) m_normalEatNum++;
                if(mon.getType()==GameGlobal::ELITE) {
                    m_eliteEatNum++;
                    m_buffStartTime=QDateTime::currentMSecsSinceEpoch();
                    m_buffActive=true;
                }
                if(mon.getType()==GameGlobal::SPECIAL) m_specialEatNum++;

                m_monsterList.removeAt(i);
                if(m_monsterList.size()<GameGlobal::getMaxMonsterCount()/2){
                    for(int j=m_monsterList.size();j<GameGlobal::getMaxMonsterCount();j++){
                        spawnMonster(canvasW,canvasH);
                    }
                }

                int randRate=RandomUtil::randInt(1,100);
                GameGlobal::SymbiosisMode createMode;
                if(mon.getType()==GameGlobal::ELITE&&randRate<=40)createMode=GameGlobal::SYMBIO_PERM;
                else if(mon.getType()==GameGlobal::SPECIAL&&randRate<=60)createMode=GameGlobal::SYMBIO_ABSORB;
                else if(randRate<=25)createMode=GameGlobal::SYMBIO_TEMP;
                else return;
                SymbiosisCell newSymCell;
                newSymCell.initSymbiosis(mon.getType(),mon.getX(),mon.getY());
                newSymCell.setSymbiosisMode(createMode);
                newSymCell.setBornTime(now);
                m_player.addSymbiosisCell(newSymCell);

                break;
            }
        }
    }
}
bool GameManager::rollbackTolastNode(){
    bool ret=m_player.rollbackToLastNode();
    if(!ret)return false;
    GhostCell newGhost;
    newGhost.initGhost(m_player.getX(),m_player.getY(),m_player.getSize(),m_player.getCurrentLaw());
    newGhost.setBornTime(QDateTime::currentMSecsSinceEpoch());
    m_ghostList.append(newGhost);
    return true;
}
bool GameManager::rollbackToAssignNode(int index){
    bool ret=m_player.rollbackToAssignNode(index);
    if(!ret)return false;
    GhostCell newGhost;
    newGhost.initGhost(m_player.getX(),m_player.getY(),m_player.getSize(),m_player.getCurrentLaw());
    newGhost.setBornTime(QDateTime::currentMSecsSinceEpoch());
    m_ghostList.append(newGhost);
    return true;
}
int GameManager::getHistoryNodeTotal()const{
    return m_ghostList.size();
}
const QList<GhostCell>& GameManager::getGhostList()const{
    return m_ghostList;
}