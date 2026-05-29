#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include "Global/GlobalDefine.h"
#include "Global/GlobalTool.h"
#include "Global/LogTool.h"

/**
 * @brief 全局JSON配置解析器 【静态内核】
 * 功能：加载策划规定的15份JSON配置，实现项目**零硬编码**
 * 重构升级：JSON格式错误、字段缺失、文件空内容等异常全覆盖，输出中文日志并启用兜底值
 */
class ConfigParser : public QObject
{
    Q_OBJECT
public:
    static ConfigParser* GetInstance();

    /**
     * @brief 加载全部15份JSON配置（程序启动后优先执行）
     */
    void LoadAllConfig();

    // ===================== 对外读取接口（按配置文件顺序对应） =====================
    QVector<Global::CellBaseProp> GetCellBaseConfig() const;
    Global::MigrateRule GetMigrateRuleConfig() const;
    QJsonArray GetEcoLayerConfig() const;
    QJsonObject GetMutateConfig() const;
    QJsonObject GetOrganelleDiffConfig() const;
    QJsonObject GetInterSpeciesConfig() const;
    QJsonObject GetGroupBioFilmConfig() const;
    QJsonObject GetMetabolismConfig() const;
    QJsonArray GetEvolveStyleConfig() const;
    QJsonObject GetEcoEventConfig() const;
    QJsonObject GetSupplyItemConfig() const;
    QJsonObject GetLineageConfig() const;
    QJsonObject GetAiBehaviorConfig() const;
    QJsonObject GetUiTipsConfig() const;
    QJsonObject GetChemotaxisSignalConfig() const;

private:
    ConfigParser() = default;
    static ConfigParser* m_instance;

    // 15份配置私有加载函数
    void LoadCellBase();
    void LoadMigrateRule();
    void LoadEnvFactor();
    void LoadGeneMutate();
    void LoadOrganelleDiff();
    void LoadInterSpecies();
    void LoadGroupBioFilm();
    void LoadMetabolism();
    void LoadEvolveStyle();
    void LoadEcoEvent();
    void LoadSupplyItem();
    void LoadLineage();
    void LoadAiBehavior();
    void LoadUiTips();
    void LoadChemotaxisSignal();

    // 配置缓存容器
    QVector<Global::CellBaseProp>   m_cellBaseList;
    Global::MigrateRule             m_migrateRule;
    QJsonArray                      m_ecoLayerArr;
    QJsonObject                     m_mutateObj;
    QJsonObject                     m_organelleObj;
    QJsonObject                     m_interSpeciesObj;
    QJsonObject                     m_groupBioFilmObj;
    QJsonObject                     m_metabolismObj;
    QJsonArray                      m_evolveStyleArr;
    QJsonObject                     m_ecoEventObj;
    QJsonObject                     m_supplyItemObj;
    QJsonObject                     m_lineageObj;
    QJsonObject                     m_aiBehaviorObj;
    QJsonObject                     m_uiTipsObj;
    QJsonObject                     m_chemoSignalObj;
};

#endif // CONFIGPARSER_H