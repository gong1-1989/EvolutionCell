#include "SaveManager.h"

SaveManager& SaveManager::getInstance()
{
    static SaveManager ins;
    return ins;
}

QString SaveManager::getSaveDir() const
{
    // 存档目录：程序运行目录下 /save/
    return QDir::currentPath() + "/save";
}

bool SaveManager::checkAndCreateDir() const
{
    QDir dir(getSaveDir());
    if (!dir.exists())
    {
        bool ret = dir.mkdir(".");
        if (!ret)
        {
            qWarning() << "存档目录创建失败！";
            return false;
        }
        qDebug() << "自动创建存档目录：" << getSaveDir();
    }
    return true;
}

QString SaveManager::createNewSave(const QJsonObject& rootData)
{
    if (!checkAndCreateDir())
        return "";

    // 文件名：时间戳.json 保证唯一
    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
    QString timeStr = QString::number(timestamp);
    QString filePath = getSaveDir() + "/" + timeStr + ".json";

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "创建存档文件失败：" << filePath;
        return "";
    }

    // 根节点追加版本号
    QJsonObject saveRoot;
    saveRoot["save_version"] = m_saveVersion;
    saveRoot["game_data"] = rootData;

    QJsonDocument doc(saveRoot);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    qDebug() << "存档成功：" << filePath;
    return filePath;
}

QJsonObject SaveManager::loadSave(const QString& filePath)
{
    QJsonObject emptyObj;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "读取存档失败，文件不存在：" << filePath;
        return emptyObj;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    if (doc.isNull() || !doc.isObject())
    {
        qWarning() << "存档文件JSON格式损坏：" << filePath;
        return emptyObj;
    }

    QJsonObject root = doc.object();
    // 版本兼容校验（可后续扩展多版本适配）
    QString ver = root["save_version"].toString("1.0");
    if (ver != m_saveVersion)
    {
        qWarning() << "存档版本不匹配，当前版本：" << m_saveVersion << " 存档版本：" << ver;
    }

    return root["game_data"].toObject();
}

QList<QString> SaveManager::getSaveFileList() const
{
    QList<QString> fileList;
    if (!checkAndCreateDir())
        return fileList;

    QDir dir(getSaveDir());
    // 仅筛选 .json 存档文件
    dir.setNameFilters(QStringList() << "*.json");
    dir.setFilter(QDir::Files);

    for (const QString& fileName : dir.entryList())
    {
        fileList.append(getSaveDir() + "/" + fileName);
    }
    return fileList;
}

QString SaveManager::getSaveFullPath(const QString &fileName)const{
    return getSaveDir()+"/"+fileName;
}

bool SaveManager::deleteSave(const QString &fullPath){
    QFile file(fullPath);
    if(!file.exists())return false;
    return file.remove();

}