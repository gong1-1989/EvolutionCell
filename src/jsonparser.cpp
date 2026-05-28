#include "jsonparser.h"

JsonParser::JsonParser(QObject *parent)
    : QObject{parent}
{}

QJsonDocument JsonParser::loadJsonFile(const QString &filepath){
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<"[加载配置] JSON解析失败，无法打开文件："<<filepath;
        return QJsonDocument();
    }
    QByteArray fileData=file.readAll();
    file.close();
    QJsonParseError err;
    QJsonDocument doc=QJsonDocument::fromJson(fileData,&err);
    if(err.error!=QJsonParseError::NoError){
        qDebug()<<"[加载配置]JSON语法错误，无法打开文件："<<err.errorString()<<"文件："<<filepath;
        return QJsonDocument();
    }
    return doc;
}

QJsonObject JsonParser::getJsonObject(const QJsonDocument &doc){
    if(!doc.isObject()){
        qDebug()<<"[加载配置]doc不是Object类型，请检查";
        return QJsonObject();
    }
    return doc.object();
}

QJsonArray JsonParser::getJsonArray(const QJsonDocument &doc){
    if(!doc.isArray()){
        qDebug()<<"[加载配置]doc不是QJsonArray类型，请检查";
        return QJsonArray();
    }
    return doc.array();
}

QString JsonParser::getString(const QJsonObject &obj, const QString &key, const QString &defVal){
    if(!obj.contains(key)||!obj.value(key).isString()){
        qDebug()<<"[加载配置]key不存在或者不是STring类型，请检查key:"<<key<<"，使用默认值"<<defVal;
        return defVal;
    }
    return obj.value(key).toString();
}

double JsonParser::getDouble(const QJsonObject &obj, const QString &key, double defVal){
    if(!obj.contains(key)||!obj.value(key).isDouble()){
        qDebug()<<"[加载配置]key不存在或者不是double类型，请检查key:"<<key<<"，使用默认值"<<defVal;
        return defVal;
    }
    return obj.value(key).toDouble();
}

int JsonParser::getInt(const QJsonObject &obj, const QString &key, int defVal){
    if(!obj.contains(key)||!obj.value(key).isDouble()){
        qDebug()<<"[加载配置]key不存在或者不是int类型，请检查key:"<<key<<"，使用默认值"<<defVal;
        return defVal;
    }
    return obj.value(key).toInt();
}

bool JsonParser::getBool(const QJsonObject &obj, const QString &key, bool defVal){
    if(!obj.contains(key)||!obj.value(key).isBool()){
        qDebug()<<"[加载配置]key不存在或者不是bool类型，请检查key:"<<key<<"，使用默认值"<<defVal;
        return defVal;
    }
    return obj.value(key).toBool();
}