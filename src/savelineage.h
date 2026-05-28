#ifndef SAVELINEAGE_H
#define SAVELINEAGE_H

#include <QObject>
#include <Qstring>
#include <QDir>
#include <QDebug>
#include "jsonparser.h"
/**
 * @brief The SaveLineage class 本地谱系存档管理：5个独立槽位
 */
class SaveLineage : public QObject
{
    Q_OBJECT
public:
    explicit SaveLineage(QObject *parent = nullptr);
    bool saveGame(int saveSlot,const QJsonDocument& doc);
    QJsonDocument loadGame(int saveSlot);
private:
    QString getSaveFullPath(int saveSlot);
    const QString m_saveRoot="LineageSave/";
    const int m_maxSlot=5;
};

#endif // SAVELINEAGE_H
