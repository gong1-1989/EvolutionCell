#include "organellfe.h"

OrganelleMgr::OrganelleMgr(QObject *parent)
    : QObject{parent}
    ,m_totipotency(100)
    ,m_diffCount(0)
    ,m_diffTencyWeight(0.0)
    ,m_diffBaseProb(0.02)
{
    connect(EventBus::getInstance(),&EventBus::globalEvent,this,[this](const QString& evt,const QVariant& data){
        if(evt==GlobalEvent::CMD_SET_DIFF_WEIGHT){
            onSetDiffWeight(data);
        }
    });
    EventBus::getInstance()->dispatchEvent(GlobalEvent::SYS_CONFIG_LOADED,"细胞分配配置");
}

void OrganelleMgr::loadDiffConfig(const QJsonDocument &doc){
    QJsonObject rootObj=JsonParser::getJsonObject(doc);
    if(rootObj.isEmpty()){
        qDebug()<<"[细胞器加载] 加载失败";
        return;
    }
    QJsonArray diffArr=rootObj.value("分化类型规则").toArray();
    for(const auto& item:diffArr){
        QJsonObject obj=item.toObject();
        DiffRule rule;
        rule.diffLevelName=JsonParser::getString(obj,"分化等级");
        rule.reatinRate=JsonParser::getInt(obj,"全能性保留比例",100);
        rule.consumeEnergy=JsonParser::getInt(obj,"单次能耗消耗",0);
        rule.allowOrganelle=JsonParser::getString(obj,"允许调整细胞器");
        rule.isEvolveIrreversible=JsonParser::getBool(obj,"是否触发演化不可逆",false);
        m_diffRuleList.append(rule);
    }
    QJsonArray coreArr=rootObj.value("核心细胞器列表").toArray();
    for(auto item:coreArr){
        QJsonObject obj=item.toObject();
        Organelle org;
        org.name=JsonParser::getString(obj,"名称");
        org.type=OrganelleType::Core;
        org.isDiscarded=false;
        org.powerLoss=JsonParser::getInt(obj,"舍弃后全能性扣除");
        org.isIrreversible=JsonParser::getBool(obj,"是否永久不可逆");
        m_organelleList.append(org);
    }
    QJsonArray secArr=rootObj.value("次要细胞器列表").toArray();
    for(auto item:secArr){
        QJsonObject obj=item.toObject();
        Organelle org;
        org.name=JsonParser::getString(obj,"名称");
        org.type=OrganelleType::Secondary;
        org.isDiscarded=false;
        if(obj.contains("舍弃后移动速度降低")) org.powerLoss=JsonParser::getInt(obj,"舍弃后移动速度降低");
        else if(obj.contains("舍弃后摄食效率降低")) org.powerLoss=JsonParser::getInt(obj,"舍弃后摄食效率降低");
        else if(obj.contains("舍弃后防御降低")) org.powerLoss=JsonParser::getInt(obj,"舍弃后防御降低");
        org.isIrreversible=JsonParser::getBool(obj,"是否永久不可逆");
        m_organelleList.append(org);
    }
    qDebug()<<"[细胞器加载] 分化规则 + 细胞器列表 加载完成";
}

int OrganelleMgr::getTotipotency()const{
    return m_totipotency;
}

bool OrganelleMgr::reachDiffMaxLimit()const{
    return m_diffCount>=m_maxDiffCount;
}

int OrganelleMgr::getDiffCount()const{
    return m_diffCount;
}
int OrganelleMgr::getDiffEnergYCost(DiffLevel level)const{
    int idx=static_cast<int>(level);
    return (idx>=0&&isx<m_diffRuleList.size())?m_diffRuleList[idx].consumeEnergy:0;
}
qreal OrganelleMgr::getDIffBaseProb()const{return m_diffBaseProb;}

void OrganelleMgr::onSetDiffWeight(const QVariant &data){
    qreal weight=data.toDouble();
    m_diffTencyWeight=qBound(0.0,weight,1.0);
    qDebug()<<"[指令]分配倾向权重已更新："<<m_diffTencyWeight;
}
bool OrganelleMgr::doDifferentiate(DiffLevel level, qreal &cellEnergy){
    if(reachDiffMaxLimit()){
        qDebug()<<"[分化] 已达最大分化次数，禁止分化";
        return false;
    }
    int idx=static_cast<int>(level);
    if(idx<0||idx>=m_diffRuleList.size()){
        qDebug()<<"[分化]等级异常，分化失败";
        return false;
    }
    int cost=m_diffRuleList[idx].consumeEnergy;
    if(cellEnergy-cost<=0) {
        qDebug()<<"[分化]能量不足，分化失败";
        return false;
    }
    cellEnergy-=cost;
    int newRetain=m_diffRuleList[idx].reatinRate;
    if(m_totipotency>newRetain){
        m_totipotency=newRetain;
        EventBus::getInstance()->dispatchEvent(GlobalEvent::STATE_TOTIPOTENCY_CHANGE,m_totipotency);
    }
    m_diffCount++;    
    qDebug()<<"[AI分化] 全性能："<<m_totipotency<<"，累计次数："<<m_diffCount;
    EventBus::getInstance()->dispatchEvent(GlobalEvent::BEHAVE_CELL_DIFF,(int)level);
    return true;
}

bool OrganelleMgr::discardOrganelle(const QString &organelleName){
    for(auto org:m_organelleList){
        if(org.name==organelleName&&!org.isDiscarded){
            if(org.type==OrganelleType::Core){
                m_totipotency-=org.powerLoss;
                qDebug()<<"[AI细胞器] 舍弃核心细胞器："<<organelleName;
                EventBus::getInstance()->dispatchEvent(GlobalEvent::STATE_TOTIPOTENCY_CHANGE,m_totipotency);
            }else{
                //预留属性变化

                qDebug()<<"[AI细胞器] 舍弃次要细胞器："<<organelleName;
            }
            org.isDiscarded=true;
            return true;
        }
    }
    return false;
}

void OrganelleMgr::setTotipotency(int val){
    m_totipotency=qBound(0,val,100);
}