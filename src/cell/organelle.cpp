#include "organellfe.h"

OrganelleMgr::OrganelleMgr(QObject *parent)
    : QObject{parent}
    ,m_totipotency(100)
    ,m_diffCount(0)
    ,m_diffTencyWeight(0.0)
{}

void OrganelleMgr::loadDiffConfig(const QJsonDocument &doc){
    QJsonObject rootObj=JsonParser::getJsonObject(doc);
    if(rootObj.isEmpty()){
        qDebug()<<"[细胞器加载] 加载失败";
        return;
    }
    m_diffRuleArr=rootObj.value("分化类型规则").toArray();
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

void OrganelleMgr::setDiffTendency(qreal weight){
    m_diffTencyWeight=qBound(0.0,weight,1.0);
}
bool OrganelleMgr::doDifferentiate(DiffLevel level){
    if(reachDiffMaxLimit()){
        qDebug()<<"[分化] 已达最大分化次数，禁止分化";
        return false;
    }
    int targetRetain=100;
    switch (level) {
    case DiffLevel::MicroDiff:
        targetRetain=90;
        break;
    case DiffLevel::MiddleDiff:
        targetRetain=70;
        break;
    case DiffLevel::DeepSpecialize:
        targetRetain=25;
        break;
    }
    m_totipotency=targetRetain;
    m_diffCount++;
    qDebug()<<"[AI分化] 全性能："<<m_totipotency<<"，累计次数："<<m_diffCount;
    return true;
}

bool OrganelleMgr::discardOrganelle(const QString &organelleName){
    for(auto org:m_organelleList){
        if(org.name==organelleName&&!org.isDiscarded){
            if(org.type==OrganelleType::Core){
                m_totipotency-=org.powerLoss;
                org.isDiscarded=true;
                qDebug()<<"[AI细胞器] 舍弃核心细胞器："<<organelleName;
            }else{
                //预留属性变化

                org.isDiscarded=true;
                qDebug()<<"[AI细胞器] 舍弃次要细胞器："<<organelleName;
            }
            return true;
        }
    }
}

void OrganelleMgr::setTotipotency(int val){
    m_totipotency=qBound(0,val,100);
}