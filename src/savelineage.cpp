#include "savelineage.h"

SaveLineage::SaveLineage(QObject *parent)
    : QObject{parent}
{
    QDir dir(m_saveRoot);
    if(!dir.exists()) dir.mkpath(".");
}

bool SaveLineage::saveGame(int saveSlot, const QJsonDocument &doc){
    if(saveSlot<0||saveSlot>=5){
        qDebug()<<"[谱系存档] 存储失败 槽位超出范围（0-4）";
        return false;
    }
    QString path=getSaveFullPath(saveSlot);
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        qDebug()<<"[谱系存档] 写入失败"<<path;
        return false;
    }
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    qDebug()<<"[谱系存档] 存储成功 槽位："<<saveSlot;
    return true;
}

QJsonDocument SaveLineage::loadGame(int saveSlot){
    QString path=getSaveFullPath(saveSlot);
    return JsonParser::loadJsonFile(path);
}
QString SaveLineage::getSaveFullPath(int saveSlot){
    return QString("%1/save_%2.json").arg(m_saveRoot).arg(saveSlot);
}
