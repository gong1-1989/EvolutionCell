#ifndef ORGANELLFEH_H
#define ORGANELLFEH_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QDebug>
#include "jsonparser.h"
#include "Utils/randomutil.h"
#include "eventbus.h"
/**
 * @brief The DiffLevel enum 分化等级枚举
 */
enum class DiffLevel{
    MicroDiff,                      //微分化
    MiddleDiff,                     //中度分化
    DeepSpecialize                  //深度分化
};
/**
 * @brief The OrganelleType enum 细胞器类型
 */
enum class OrganelleType{
    Core,                           //核心细胞器（不可逆）
    Secondary                       //次要细胞器（可恢复）
};
/**
 * @brief The Organelle 单个细胞器实体
 */
struct Organelle
{
    QString name;                   //细胞器名字
    OrganelleType type;             //类型：核心/次要
    bool isDiscarded;               //是否已被舍弃
    int powerLoss;                  //舍弃带来的属性损耗
    bool isIrreversible;            //是否永久不可逆
};
/**
 * @brief The OrganelleMgr class 细胞器实体+分化规则管理
 */
class OrganelleMgr : public QObject
{
    Q_OBJECT
public:
    explicit OrganelleMgr(QObject *parent = nullptr);

    /**
     * @brief getTotipotency 获取当前全性能百分比
     */
    int getTotipotency()const;

    /**
     * @brief reachDiffMaxLimit 检查是否达到分化上限
     * @return true=达到上限
     */
    bool reachDiffMaxLimit()const;

    /**
     * @brief getDiffCount 获取当前分化次数
     */
    int getDiffCount()const;

    /**
     * @brief setDiffTendency 玩家引导权重（带权限锁，仅叠加倾向）
     * @param weight 玩家权重
     */
    void setDiffTendency(qreal weight);

    /**
     * @brief loadDiffConfig 全局初始化
     * @param doc QJsonDocument数据
     */
    void loadDiffConfig(const QJsonDocument& doc);
private:
    /**
     * @brief doDifferentiate 执行细胞分化
     * @param level 分化等级
     * @return true=分化成功
     */
    bool doDifferentiate(DiffLevel level);

    /**
     * @brief discardOrganelle 舍弃指定细胞器
     * @param organelleName 指定细胞器的名字
     * @return true=舍弃成功
     */
    bool discardOrganelle(const QString& organelleName);

    /**
     * @brief setTotipotency 设置全性能
     * @param val
     */
    void setTotipotency(int val);

    QJsonDocument m_diffRuleArr;
    QList<Organelle> m_organelleList;
    int m_totipotency;                          //全性能
    int m_diffCount;                            //本局已分化次数
    const int m_maxDiffCount=8;                 //单局最大分化8次
    qreal m_diffTencyWeight;                    //玩家引导权重0-1.0（仅影响分化倾向，不直接触发）
};

#endif // ORGANELLFEH_H
