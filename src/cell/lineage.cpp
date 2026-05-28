#include "lineage.h"

LineageTree::LineageTree(QObject *parent)
    : QObject{parent}
{}

void LineageTree::loadLineageConfig(const QJsonDocument &doc){
    m_lineageCfg=JsonParser::getJsonObject(doc);
    m_maxSaveRound=JsonParser::getInt(m_lineageCfg,"本地存档最大轮数",50);
    qDebug()<<"[谱系模块] 配置加载完成，最大存档轮数："<<m_maxSaveRound;
}
bool LineageTree::savecuttentLineage(int saveSlot){
    if(saveSlot<0||saveSlot>=5)return false;
    QJsonDocument saveDoc;
    //组装saveDoc：本局基因、形状数据（按基因留存规则筛选）（暂占位）
    SaveLineage sl;
    bool ret=sl.saveGame(saveSlot,saveDoc);
    if(ret) EventBus::getInstance()->dispatchEvent(GlobalEvent::EVT_LINEAGE_UPDATE);
    return ret;
}
bool LineageTree::loadHistoryLineage(int saveSlot){
    SaveLineage sl;
    QJsonDocument doc=sl.loadGame(saveSlot);
    if(doc.isNull()){
        qDebug()<<"[谱系] 无历史存档，使用本源初始基因";
        return false;
    }
    //按规则继承：显性永久、隐形3轮、缺陷不继承（暂占位）
    qDebug()<<"[谱系] 读取历史基因，完成轮回继承";
    return true;
}
int LineageTree::getNextRoundEnergAdd(int surviveSecond){
    QJsonArray gradientArr=m_lineageCfg.value("生存时长奖励梯度").toArray();
    for(const auto&item:gradientArr){
        QJsonObject obj=item.toObject();
        QJsonArray timeRange=obj.value("时长区间(秒)").toArray();
        int tMin=timeRange.at(0).toInt();
        int tMax=timeRange.at(1).toInt();
        if(surviveSecond>=tMin&&surviveSecond<=tMax)
            return JsonParser::getInt(obj,"初始营养加成 ",0);
    }
    return 0;

}