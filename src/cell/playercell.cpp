#include "playercell.h"
#include "Utils/randomutil.h"
PlayerCell::PlayerCell()
    :m_x(0),m_y(0)
    ,m_size(GameGlobal::getPlayerInitSize())
    ,m_speed(GameGlobal::getPlayerSpeed())
    ,m_hasSpeedBuff(false)
    ,m_hasSpeedDebuff(false)
    ,m_lawType(GameGlobal::LAW_FISSION)
    ,m_decomposeLv(GameGlobal::DECOMPOSE_NONE)
    ,m_currentDecomposeRisk(0)
    ,m_critBonus(0.0),m_atkBonus(0.0),m_speedLoss(0.0)
    ,m_symbiosisCount(0),m_geneRejectValue(0)
{}

void PlayerCell::move(bool w, bool a, bool s, bool d, int canvasW, int canvasH){
    qreal speed=GameGlobal::getPlayerSpeed()*getGeneSpeedRatio()-m_speedLoss;
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
void PlayerCell::initLifeLaw(GameGlobal::LifeLaw law){
    m_lawType=law;
    m_critBonus=0.0;
    m_atkBonus=0.0;
    m_speedLoss=0.0;
    switch(law){
    case GameGlobal::LAW_FISSION:
        //裂变”初始攻击小幅加成
        m_atkBonus+=0.12;
        break;
    case GameGlobal::LAW_SYMBIOSIS:
        //共生：无初始损耗，偏向生存
        m_symbiosisCount=0;
        break;
    case GameGlobal::LAW_ILLUSION:
        //虚妄：初始暴击加成，移速小幅优势
        m_critBonus+=0.15;
        break;
    }
}
bool PlayerCell::doDecompose(GameGlobal::DecomposeLevel targetLv){
    //规则1：只能逐级加深拆解，不能降级复原（策划取舍不可逆）
    if(targetLv<=m_decomposeLv)return false;
    int newRisk=GameGlobal::getDecomposerRisk(targetLv);
    m_currentDecomposeRisk+=newRisk;
    switch(targetLv){        
        case GameGlobal::DECOMPOSE_LIGHT:
            m_speedLoss+=GameGlobal::getLightDocmposeSpeedLoss();
            m_critBonus+=GameGlobal::getLightDocmposeCritGain();
            break;
        case GameGlobal::DECOMPOSE_DEEP:
            m_speedLoss+=GameGlobal::getDeepDocmposeHPLoss();
            m_atkBonus+=GameGlobal::getDeepDocmposeAtkGain();
            break;
        case GameGlobal::DECOMPOSE_FULL:
            m_atkBonus+=GameGlobal::getFullDocmposeExtremeAt();
            break;
        default:
            return false;
            break;
    }
    m_decomposeLv=targetLv;
    return true;
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
qreal PlayerCell::getCritBonus()const{
    return m_critBonus;
}
qreal PlayerCell::getAttackBouns()const{
    return m_atkBonus;
}
qreal PlayerCell::getSpeedModify()const{
    return -m_speedLoss;
}
GameGlobal::LifeLaw PlayerCell::getCurrentLaw()const{
    return m_lawType;
}
GameGlobal::DecomposeLevel PlayerCell::getDecomposeLevel()const{
    return m_decomposeLv;
}
int PlayerCell::getDecomposeRisk()const{
    return m_currentDecomposeRisk;
}
bool PlayerCell::addSymbiosisCell(const SymbiosisCell &cell){
    int maxCount=GameGlobal::getMaxSymbiosisCount();
    if(m_symbiosisList.size()>=maxCount)return false;
    m_symbiosisList.append(cell);
    m_geneRejectValue+=GameGlobal::getSingleRejectValue();
    return true;
}
void PlayerCell::clealExpiredSymbiosis(qint64 nowTime){
    for(int i=m_symbiosisList.size()-1;i>=0;--i){
        if(m_symbiosisList[i].isTempExpired(nowTime)){
            m_geneRejectValue-=GameGlobal::getSingleRejectValue();
            m_symbiosisList.removeAt(i);
        }
    }
}
GameGlobal::RejectLevel PlayerCell::getcurrentRejectLevel()const{
    int warnLimit=GameGlobal::getRejectWarningThreshold();
    int dangerLimit=GameGlobal::getRejectDangerThreshold();
    if(m_geneRejectValue>=dangerLimit)return GameGlobal::REJECT_DANGER;
    else if(m_geneRejectValue>=warnLimit)return GameGlobal::REJECT_WARNING;
    return GameGlobal::REJECT_SAFE;
}
qreal PlayerCell::getRejectAttrModify()const{
    auto level=getcurrentRejectLevel();
    switch (level) {
    case GameGlobal::REJECT_WARNING:
        return 0.85;
        break;
    case GameGlobal::REJECT_DANGER:
        return 0.6;
        break;
    default:
        return 1.0;
        break;
    }
}
int PlayerCell::getSymbiosisCount() const{
    return m_symbiosisList.size();
}
const QList<SymbiosisCell>& PlayerCell::getSymbiosisList() const{
    return m_symbiosisList;
}
void PlayerCell::updateSymbiosisFollow(qreal playerX, qreal playerY){
    qreal range=GameGlobal::getSymbiosisFollowRange();
    for(auto& cell:m_symbiosisList){
        cell.followUpdate(playerX,playerY,range);
    }
}