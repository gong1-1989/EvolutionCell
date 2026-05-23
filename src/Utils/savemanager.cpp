#include "savemanager.h"
#include <QFile>
#include <QDebug>

SaveManager::SaveManager(QObject *parent)
    : QObject{parent}
{}
SaveManager& SaveManager::getInstance(){
    static SaveManager ins;
    return ins;
}
QString SaveManager::getSlotPath(int slot) const{
    return QString("save/save_slot%1.json").arg(slot);
}
bool SaveManager::saveToSlot(int slot, const QJsonObject &data){
    if(slot<1||slot>SLOT_COUNT) return false;
    QString path=getSlotPath(slot);
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        qDebug()<<"存档位："<<slot<<"保存失败！";
        return false;
    }
    QJsonObject saveObj;
    saveObj["timestamp"]=QDateTime::currentDateTime().toString("yyyy-MM-dd HH::mm");
    saveObj["game_data"]=data;
    QJsonDocument doc(saveObj);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}
QJsonObject SaveManager::loadFromSlot(int slot){
    if(slot<1||slot>SLOT_COUNT)return QJsonObject();
    QString path=getSlotPath(slot);
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        qDebug()<<"存档文件加载失败："<<path;
        return QJsonObject();
    }
    QByteArray raw=file.readAll();
    file.close();
    QJsonDocument doc=QJsonDocument::fromJson(raw);
    if(!doc.isObject()) return QJsonObject();
    return doc.object()["game_data"].toObject();
}
bool SaveManager::slotHasSave(int slot){
    if(slot<1||slot>SLOT_COUNT)return false;
    return QFile::exists(getSlotPath(slot));
}
QString SaveManager::getSaveTime(int slot){
    if(!slotHasSave(slot))return "空存档";
    QJsonObject obj=loadFromSlot(slot);
    if(obj.isEmpty())return "损坏存档";
    return obj["timestamp"].toString("无记录");
}
bool SaveManager::deleteSlotSave(int slot){
    if(!slotHasSave(slot))return true;
    return QFile::remove(getSlotPath(slot));
}
SaveBriefInfo SaveManager::getSlotBriefInfo(int slot){
    SaveBriefInfo info;
    info.exist=false;
    if(!slotHasSave(slot)) return info;
    QString path=getSlotPath(slot);
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))return info;
    QByteArray raw=file.readAll();
    file.close();
    QJsonDocument doc=QJsonDocument::fromJson(raw);
    if(!doc.isObject()) return info;
    info.exist=true;
    QJsonObject root=doc.object();
    info.saveTime=root["timestamp"].toString("未知时间");
    QJsonObject gameData=root["game_data"].toObject();
    QJsonObject player=gameData["player"].toObject();
    info.cellSize=player["size"].toInt(0);
    info.eatTotal=gameData["stat"].toObject()["total_eat"].toInt(0);
    return info;
}