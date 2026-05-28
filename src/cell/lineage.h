#ifndef LINEAGE_H
#define LINEAGE_H

#include <QObject>
#include <qDebug>
#include "savelineage.h"
#include "jsonparser.h"
#include "eventbus.h"

/**
 * @brief The Lineage class 谱系系统：基因继承、谱系存档、轮回初始能量加成
 */
class LineageTree : public QObject
{
    Q_OBJECT
public:
    explicit LineageTree(QObject *parent = nullptr);
    /**
     * @brief loadLineageConfig 全局加载配置
     * @param doc QJsonDocument数据
     */
    void loadLineageConfig(const QJsonDocument& doc);

    /**
     * @brief savecuttentLineage 保存历史谱系到指定槽位
     * @param saveSlot 存档槽位
     * @return true=存档成功
     */
    bool savecuttentLineage(int saveSlot);

    /**
     * @brief loadHistoryLineage读存历史谱系，执行基因继承
     * @param saveSlot 槽位
     * @return true=读取成功
     */
    bool loadHistoryLineage(int saveSlot);

    /**
     * @brief getNextRoundEnergAdd 根据生成时长，执行下一局初始能量加成
     * @param surviveSecond
     */
    int getNextRoundEnergAdd(int surviveSecond);
private:
    QJsonObject m_lineageCfg;
    int m_maxSaveRound=50;
};

#endif // LINEAGE_H
