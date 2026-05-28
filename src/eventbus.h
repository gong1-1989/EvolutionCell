#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <QObject>
#include <QVariant>
#include <QString>
/**
 * @brief The EventBus class 全局事件总线，模块解耦中心
 */
class EventBus : public QObject
{
    Q_OBJECT
    static EventBus* m_ins;
    explicit EventBus(QObject *parent = nullptr);
public:
    //全局单例
    static EventBus* getInstance();
    //销毁单例
    static void destroyInstance();

    /**
     * @brief dispatchEvent 分发全局事件
     * @param evtName 事件名称（字符串）
     * @param data 事件附加数据（通用变体）
     */
    void dispatchEvent(const QString& evtName,const QVariant&data=QVariant());

signals:
    /**
     * @brief globalEvent 全局事件信号：所有事件统一触发此信号
     * @param evtName 事件名称（字符串）
     * @param data 事件附加数据（通用变体）
     */
    void globalEvent(const QString& evtName,const QVariant&data);

};
//全局事件常量（统一管理，避免魔法字符串）
namespace GlobalEvent {
const QString EVT_CELL_UPDATE="EVT_CELL_UPDATE";        //细胞状态更新
const QString EVT_CELL_DEATH="EVT_CELL_DEATH";          //细胞凋零
const QString EVT_MIGRATE  ="EVT_MIGRATE";              //跨层迁徙
const QString EVT_HABITAT_CRASH="EVT_HABITAT_CRASH";    //生境崩溃

const QString EVT_CELL_DIFF="EVT_CELL_DIFF";            //细胞发生分化
const QString EVT_GENE_MUTATE="EVT_GENE_MUTATE";        //发生基因突变
const QString EVT_HGT="EVT_HGT";                        //水平基因转移（科普弹窗触发源）
const QString EVT_LINEAGE_UPDATE="EVT_LINEAGE_UPDATE";  //谱系数据更新
}

#endif // EVENTBUS_H
