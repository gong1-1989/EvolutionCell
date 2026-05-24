#include "savemanager.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QFileInfoList>
const QString SAVE_DIR_PATH="save";
const QString SAVE_FILE_PREFIX="save_";
const QString SAVE_FILE_SUFFIX=".json";
SaveManager::SaveManager(QObject *parent)
    : QObject{parent}
{}
SaveManager& SaveManager::getInstance(){
    static SaveManager ins;
    return ins;
}
QString SaveManager::generatetimeSaveName()const{
    QString timeStr=QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    return QString("%1%2%3").arg(SAVE_FILE_PREFIX).arg(timeStr).arg(SAVE_FILE_SUFFIX);
}
QString SaveManager::getSlotPath(const QString &filePath) const{
    return QString("%1/%2").arg(SAVE_DIR_PATH).arg(filePath);
}
QString SaveManager::saveToSlot( const QJsonObject &data){
    QDir saveDir(SAVE_DIR_PATH);
    if(!saveDir.exists()){
        bool createRet=saveDir.mkpath(".");
        if(!createRet){
            qDebug()<<"存档目录创建失败，无法保存存档！";
            return "";
        }
    }
    QString path=getSlotPath(generatetimeSaveName());
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        qDebug()<<"存档失败，文件无法写入！";
        return "";
    }
    QJsonObject saveObj;
    saveObj["timestamp"]=QDateTime::currentDateTime().toString("yyyy-MM-dd HH::mm");
    saveObj["game_data"]=data;
    QJsonDocument doc(saveObj);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    qDebug()<<"存档成功！";
    return path;
}
QJsonObject SaveManager::loadFromSlot(const QString &filePath){
    if(!QDir(SAVE_DIR_PATH).exists()){
        qDebug()<<"存档目录不存在，无法读取存档！";
        return QJsonObject();
    }
    QString path=getSlotPath(filePath);
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        qDebug()<<"存档文件加载失败："<<path;
        return QJsonObject();
    }
    QByteArray raw=file.readAll();
    file.close();
    QJsonDocument doc=QJsonDocument::fromJson(raw);
    if(!doc.isObject()) {
        qDebug()<<"存档位"<<filePath<<"数据格式损坏！";
        return QJsonObject();
    }
    return doc.object()["game_data"].toObject();
}
bool SaveManager::deleteSlotSave(const QString &filePath){
    return QFile::remove(getSlotPath(filePath));
}
QList<SaveBriefInfo> SaveManager::getBriefList(){
   QList<SaveBriefInfo> briefList;
    QDir dir(SAVE_DIR_PATH);
    if(!dir.exists()){
        qDebug()<<"存档目录不存在，无法读取存档！";
        return briefList;
    }
    QFileInfoList fileList=dir.entryInfoList(QDir::Files);
    for(const QFileInfo& info:fileList){
        QString fileName=info.fileName();
        if(!fileName.startsWith(SAVE_FILE_PREFIX)||!fileName.endsWith(SAVE_FILE_SUFFIX))
            continue;
        SaveBriefInfo brief;
        brief.fileName=fileName;
        brief.exist=true;
        QFile file(info.absoluteFilePath());
        if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
            brief.saveTime="损坏存档";
            briefList.append(brief);
            file.close();
            continue;
        }
        QByteArray raw=file.readAll();
        file.close();
        QJsonDocument doc=QJsonDocument::fromJson(raw);
        if(!doc.isObject()){
            brief.saveTime="格式异常";
            briefList.append(brief);
            continue;
        }
        QJsonObject root=doc.object();
        brief.saveTime=root["timestamp"].toString("未知时间");
        QJsonObject gameData=root["game_data"].toObject();
        QJsonObject player=gameData["player"].toObject();
        brief.cellSize=player["size"].toInt();
        QJsonObject stat=gameData["stat"].toObject();
        brief.eatTotal=stat["total_eat"].toInt();
        briefList.append(brief);
    }
    std::sort(briefList.begin(),briefList.end(),[](const SaveBriefInfo&a,const SaveBriefInfo&b){
        return a.saveTime>b.saveTime;
    });
    return briefList;
}