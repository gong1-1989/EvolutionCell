#include "gene.h"

GeneMgr::GeneMgr(QObject *parent)
    : QObject{parent}
    ,m_malignantTotal(0)
    ,m_firstHGT(true)
    ,m_mutateWeight(0.0)
{}

void GeneMgr::loadMutateConfig(const QJsonDocument &doc){
    QJsonObject root =JsonParser::getJsonObject(doc);
    m_spontanepusCfg=root.value("自发突变规则").toObject();
    m_inducedcfg=root.value("诱导突变规则").toObject();
    m_malignantStackCfg=root.value("恶性突变叠加规则").toObject();
    m_hgtCfg=root.value("水平基因转移规则").toObject();
    qDebug()<<"[基因] 突变规则配置加载完成";
}
int GeneMgr::getMalignantCount()const{
    return m_malignantTotal;
}
bool GeneMgr::isPrematureAging()const{
    int limit=JsonParser::getInt(m_malignantStackCfg,"累计恶性突变上限",3);
    return m_malignantTotal>=limit;
}
int GeneMgr::getAgingEnergyAdd()const{
    return JsonParser::getInt(m_malignantStackCfg,"达到上限后能耗增幅(%)",50);
}
void GeneMgr::setMutateTendency(qreal weight){
    m_mutateWeight=qBound(0.0,weight,100.0);
}
MutateType GeneMgr::doSpontaneousMutate(){
    qreal baseProb=JsonParser::getDouble(m_spontanepusCfg,"单次触发概率(%)",0.05);
    qreal finalProb=baseProb*(1+m_mutateWeight);
    qreal randVal=RandomUtil::randInt(0,100);
    if(randVal>finalProb)return MutateType::Neutral;
    qreal typeRand=RandomUtil::randInt(0,100);
    qreal benignRate=JsonParser::getInt(m_spontanepusCfg,"良性性状占比(%)",15);
    qreal neutralRate=JsonParser::getInt(m_spontanepusCfg,"中性性状占比(%)",70);
    MutateType res=MutateType::Neutral;
    if(typeRand<benignRate) res=MutateType::Benign;
    else if(typeRand<benignRate+neutralRate) res=MutateType::Neutral;
    else{
        res=MutateType::Malignant;
        m_malignantTotal++;
    }
    return res;
}

MutateType GeneMgr::doInducedMutate(qreal stressRate){
    qreal baseProb=JsonParser::getInt(m_inducedcfg,"基础触发概率(%)",20);
    qreal addPerTen=JsonParser::getInt(m_inducedcfg,"胁迫强度每提升10%概率增幅(%)",8);
    qreal totalProb=(baseProb+(stressRate/10.0)*addPerTen)*(1+m_mutateWeight);
    qreal randVal=RandomUtil::randInt(0,100);
    if(randVal>totalProb) return MutateType::Neutral;
    qreal typeRand=RandomUtil::randInt(0,100);
    qreal benignRate=JsonParser::getInt(m_inducedcfg,"良性性状占比(%)",25);
    qreal neutralRate=JsonParser::getInt(m_inducedcfg,"中性性状占比(%)",50);
    MutateType res=MutateType::Neutral;
    if(typeRand<benignRate) res=MutateType::Benign;
    else if(typeRand<benignRate+neutralRate) res=MutateType::Neutral;
    else{
        res=MutateType::Malignant;
        m_malignantTotal++;
    }
    return res;
}

bool GeneMgr::doHorizontalGeneTransfer(){
    qreal prob=JsonParser::getDouble(m_hgtCfg,"异种接触触发概率(%)",8.0);
    qreal finalProb=prob*(1+m_mutateWeight);
    qreal randVal=RandomUtil::randInt(0,100);
    if(randVal>finalProb)return false;
    if(m_firstHGT){
        m_firstHGT=false;
        //首次触发，推送科普弹窗
        EventBus::getInstance()->dispatchEvent(GlobalEvent::EVT_HGT);
    }
    return true;
}