#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QString>
#include <QDebug>
/**
 * @brief The JsonParser class 全局JSON配置解析器
 */
class JsonParser : public QObject
{
    Q_OBJECT
public:
    explicit JsonParser(QObject *parent = nullptr);
    /**
     * @brief loadJsonFile 读取JSON文件
     * @param filepath 配置文件的相对路径
     * @return 解析后的QJsonDocument，解析失败则返回空
     */
    static QJsonDocument loadJsonFile(const QString& filepath);

    /**
     * @brief getJsonObject 读取JSON对象
     * @param doc 已解析的QJsonDocument
     * @return QJsonObject
     */
    static QJsonObject getJsonObject(const QJsonDocument& doc);

    /**
     * @brief getJsonArray 读取JSON数组
     * @param doc 已解析的QJsonDocument
     * @return QJsonArray
     */
    static QJsonArray getJsonArray(const QJsonDocument& doc);

    /**
     * @brief getString 通用读取字符串值
     * @param obj JSON对象
     * @param key 中文Key
     * @param defVal 默认值
     * @return 读取的结果QString
     */
    static QString getString(const QJsonObject& obj,const QString& key,const QString& defVal="");

    /**
     * @brief getDouble 通用读取浮点数值
     * @param obj JSON对象
     * @param key 中文Key
     * @param defVal 默认值
     * @return 读取的结果double
     */
    static double getDouble(const QJsonObject& obj,const QString& key,double defVal=0.0);

    /**
     * @brief getInt 通用读取整数值
     * @param obj JSON对象
     * @param key 中文Key
     * @param defVal 默认值
     * @return 读取的结果int
     */
    static int getInt(const QJsonObject& obj,const QString& key,int defVal=0);

    /**
     * @brief getBool 通用读取布尔值
     * @param obj JSON对象
     * @param key 中文Key
     * @param defVal 默认值
     * @return 读取的结果bool
     */
    static bool getBool(const QJsonObject& obj,const QString& key,bool defVal=false);

signals:
};

#endif // JSONPARSER_H
