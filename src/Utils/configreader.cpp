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

int ConfigReader::getInt(const QJsonObject& obj, const QString& key, int defVal) const
{
    if (!obj.contains(key) || !obj[key].isDouble())
    {
        qWarning() << "配置字段异常：" << key << "，使用默认值：" << defVal;
        return defVal;
    }
    return obj[key].toInt(defVal);
}

qreal ConfigReader::getDouble(const QJsonObject& obj, const QString& key, qreal defVal) const
{
    if (!obj.contains(key) || !obj[key].isDouble())
    {
        qWarning() << "配置字段异常：" << key << "，使用默认值：" << defVal;
        return defVal;
    }
    return obj[key].toDouble(defVal);
}

QString ConfigReader::getString(const QJsonObject& obj,const QString& key,QString strVal)const{
    if (!obj.contains(key) || !obj[key].isString())
    {
        qWarning() << "配置字段异常：" << key << "，使用默认值：" << strVal;
        return strVal;
    }
    return obj[key].toString(strVal);
}
