#include "ConfigReader.h"

ConfigReader& ConfigReader::getInstance()
{
    static ConfigReader ins;
    return ins;
}

void ConfigReader::loadAllConfig()
{
    const QString cfgPath = ":/Config/Res/Config/game_config.json";
    QFile file(cfgPath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "配置文件打开失败：" << cfgPath;
        m_rootCfg = QJsonObject();
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject())
    {
        qWarning() << "配置文件JSON格式错误";
        m_rootCfg = QJsonObject();
        return;
    }

    m_rootCfg = doc.object();
    qDebug() << "全局配置加载完成";
}

void ConfigReader::reloadAllConfig()
{
    loadAllConfig();
    qDebug() << "配置热重载完成";
}

QJsonObject ConfigReader::getRootConfig() const
{
    return m_rootCfg;
}

int ConfigReader::getInt(const QString& group, const QString& key, int defVal) const
{
    if(!m_rootCfg.contains(group)||!m_rootCfg[group].isObject()){
        qWarning()<<"配置字段异常group：" << group << "，使用默认值：" << defVal;
        return defVal;
    }
    QJsonObject obj=m_rootCfg.value(group).toObject();
    if (!obj.contains(key) || !obj[key].isDouble())
    {
        qWarning() << "配置字段异常key：" << key << "，使用默认值：" << defVal;
        return defVal;
    }
    return obj[key].toInt();
}

qreal ConfigReader::getDouble(const QString& group, const QString& key, qreal defVal) const
{
    if(!m_rootCfg.contains(group)||!m_rootCfg[group].isObject()){
        qWarning()<<"配置字段异常group：" << group << "，使用默认值：" << defVal;
        return defVal;
    }
    QJsonObject obj=m_rootCfg.value(group).toObject();
    if (!obj.contains(key) || !obj[key].isDouble())
    {
        qWarning() << "配置字段异常：" << key << "，使用默认值：" << defVal;
        return defVal;
    }
    return obj[key].toDouble();
}

QString ConfigReader::getString(const QString &group, const QString& key, QString defVal)const
{
    if(!m_rootCfg.contains(group)||!m_rootCfg[group].isObject()){
        qWarning()<<"配置字段异常group：" << group << "，使用默认值：" << defVal;
        return defVal;
    }
    QJsonObject obj=m_rootCfg.value(group).toObject();
    if (!obj.contains(key) || !obj[key].isString())
    {
        qWarning() << "配置字段异常：" << key << "，使用默认值：" << defVal;
        return defVal;
    }
    return obj[key].toString();
}

QJsonObject ConfigReader::getSubObj(const QString& group,const QString& key ,const QJsonObject& defVal)const
{
    if(!m_rootCfg.contains(group)||!m_rootCfg[group].isObject()){
        qWarning()<<"配置字段异常group：" << group;
        return defVal;
    }
    QJsonObject obj=m_rootCfg.value(group).toObject();
    if (!obj.contains(key) || !obj[key].isObject())
    {
        qWarning() << "配置字段异常：" << key << "，使用默认值：" << defVal;
        return defVal;
    }
    return obj[key].toObject();
}

