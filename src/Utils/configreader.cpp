#include "configreader.h"

ConfigReader& ConfigReader::getInstance() {
    static ConfigReader ins;
    return ins;
}
QJsonObject ConfigReader::readJsonFile(const QString &filePath){
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<"配置文件打开失败："<<filePath;
        return QJsonObject();
    }
    QByteArray data=file.readAll();
    file.close();
    QJsonDocument doc=QJsonDocument::fromJson(data);
    return doc.object();
}
bool ConfigReader::loadAllConfig(){
    m_gameObj=readJsonFile(":/Config/Res/Config/game_config.json");
    m_monsterObj=readJsonFile(":/Config/Res/Config/monster_config.json");
    m_geneObj=readJsonFile(":/Config/Res/Config/gene_config.json");
    return !m_gameObj.isEmpty()&&!m_monsterObj.isEmpty()&&!m_geneObj.isEmpty();
}
QJsonObject ConfigReader::getGemeConfig()const{
    return m_gameObj;
}
QJsonObject ConfigReader::getMonsterConfig()const{
    return m_monsterObj;
}
QJsonObject ConfigReader::getGeneConfig()const{
    return m_geneObj;
}