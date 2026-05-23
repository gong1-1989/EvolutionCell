#ifndef CONFIGREADER_H
#define CONFIGREADER_H
//JSON 读取工具
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QFile>
class ConfigReader
{
public:
    static ConfigReader& getInstance();
    bool loadAllConfig();
    QJsonObject getGemeConfig() const;
    QJsonObject getMonsterConfig()const;
    QJsonObject getGeneConfig()const;
private:
    ConfigReader()=default;
    QJsonObject m_gameObj,m_monsterObj,m_geneObj;
    QJsonObject readJsonFile(const QString& filePath);
};

#endif // CONFIGREADER_H
