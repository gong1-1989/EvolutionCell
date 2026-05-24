#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H
//存档管理器
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QDateTime>
#include <QVector>

struct SaveBriefInfo{
    QString fileName;
    bool exist=false;
    QString saveTime;
    int eatTotal=0;
    int cellSize=0;
};

class SaveManager : public QObject
{
    Q_OBJECT
public:
    static SaveManager& getInstance();
    QString saveToSlot(const QJsonObject&data);
    QJsonObject loadFromSlot(const QString& filePath);
    bool deleteSlotSave(const QString& filePath);
    QList<SaveBriefInfo> getBriefList();
private:
    explicit SaveManager(QObject *parent = nullptr);
    QString getSlotPath(const QString& filePath) const;
    QString generatetimeSaveName()const;
};

#endif // SAVEMANAGER_H
