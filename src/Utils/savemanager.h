#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H
//存档管理器
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QDateTime>

struct SaveBriefInfo{
    bool exist=false;
    QString saveTime;
    int eatTotal=0;
    int cellSize=0;
};

class SaveManager : public QObject
{
    Q_OBJECT
public:
    static constexpr int SLOT_COUNT=5;
    static SaveManager& getInstance();
    bool saveToSlot(int slot,const QJsonObject&data);
    QJsonObject loadFromSlot(int slot);
    bool slotHasSave(int slot);
    QString getSaveTime(int slot);
    bool deleteSlotSave(int slot);
    SaveBriefInfo getSlotBriefInfo(int slot);
private:
    explicit SaveManager(QObject *parent = nullptr);
    QString getSlotPath(int slot) const;

signals:
};

#endif // SAVEMANAGER_H
