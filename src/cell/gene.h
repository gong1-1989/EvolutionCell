#ifndef GENE_H
#define GENE_H

#include <QObject>
#include "jsonparser.h"
#include "Utils/randomutil.h"
#include "eventbus.h"
/**
 * @brief The MutateType enum 突变形状类型
 */
enum class MutateType{
    Benign,                 //良性突变
    Neutral,                //中性突变
    Malignant               //恶性突变
};
class Cell;     //友元声明
/**
 * @brief The GeneMgr class 基因突变系统、性状分类、恶性突变早衰规则
 */
class GeneMgr : public QObject
{
    Q_OBJECT
    friend class Cell;  //允许Cell访问私有成员、函数
public:
    explicit GeneMgr(QObject *parent = nullptr);

    /**
     * @brief getMalignantCount 获取突变次数
     */
    int getMalignantCount()const;

    /**
     * @brief isPrematureAging 检查是否早衰
     * @return true=早衰
     */
    bool isPrematureAging()const;

    /**
     * @brief getAgingEnergyAdd 早衰能耗增幅
     */
    int getAgingEnergyAdd()const;

    /**
     * @brief isFirstHGT 是否本局首次HGT（基因水平转移）
     * @return true=是
     */
    bool isFirstHGT()const;

    /**
     * @brief setMutateTendency 玩家引导权重
     * @param weight 玩家权重
     */
    void setMutateTendency(qreal weight);

    /**
     * @brief loadMutateConfig 全局配置加载
     * @param doc QJsonDocument数据
     */
    void loadMutateConfig(const QJsonDocument& doc);
private:
    /**
     * @brief doSpontaneousMutate 自发突变
     * @return 突变类型
     */
    MutateType doSpontaneousMutate();

    /**
     * @brief doInducedMutate 诱导突变
     * @param stressRate
     * @return 突变类型
     */
    MutateType doInducedMutate(qreal stressRate);

    /**
     * @brief doHorizontalGeneTransfer 水平基因转移
     * @return true=转移成功
     */
    bool doHorizontalGeneTransfer();

    QJsonObject m_spontanepusCfg;
    QJsonObject m_inducedcfg;
    QJsonObject m_malignantStackCfg;
    QJsonObject m_hgtCfg;
    int m_malignantTotal;                   //累计恶性突变次数
    bool m_firstHGT;                        //本局首次HGT标记
    qreal m_mutateWeight;                   //突变倾向权重，玩家引导0-1.0
};

#endif // GENE_H
