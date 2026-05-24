#include "gamemanager.h"

GameManager::GameManager():m_sceneInited(false)
    ,m_buffActive(false)
    ,m_gameState(GameGlobal::RUNING)
    ,m_eatCount(0)
    ,m_normalEatNum(0)
    ,m_eliteEatNum(0)
    ,m_specialEatNum(0)
{}

void GameManager::initScene(int w, int h){
    m_player.setPos(w/2.0,h/2.0);
    m_monsterList.clear();
    for(int i=0;i<GameGlobal::getMaxMonsterCount();i++){
        spawnMonster(w,h);
    }
    m_sceneInited=true;
    m_eatCount=0;
    m_gameState=GameGlobal::RUNING;

    m_normalEatNum=0;
    m_eliteEatNum=0;
    m_specialEatNum=0;
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
    for(MonsterCell &mon:m_monsterList){
        mon.move(canvasW,canvasH);
    }
    checkEat(canvasW,canvasH);
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
        }
    }
}
const PlayerCell& GameManager::getPlayer() const{
    return m_player;
}
const QList<MonsterCell>& GameManager::getMonsterList() const{
    return m_monsterList;
}

bool GameManager::saveToSaveSlot(int slot){
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
    return SaveManager::getInstance().saveToSlot(slot,data);
}
bool GameManager::loadFromSaveSlot(int slot){
    QJsonObject data=SaveManager::getInstance().loadFromSlot(slot);
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