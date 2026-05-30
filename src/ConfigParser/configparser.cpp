#include "ConfigParser.h"

ConfigParser* ConfigParser::m_instance = nullptr;
const QString MODULE_NAME = "配置解析器";

ConfigParser* ConfigParser::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new ConfigParser;
        LOG_INFO(MODULE_NAME, "配置解析器单例创建成功");
    }
    return m_instance;
}

void ConfigParser::LoadAllConfig()
{
    LOG_INFO(MODULE_NAME, "开始加载全部15份JSON配置文件");
    // 按依赖顺序加载：系统设置 → 细胞基础属性 → 生态 → 演化 → 玩法 → UI
    LoadGameSetting();
    LoadCellBase();
    LoadMigrateRule();
    LoadEnvFactor();
    LoadGeneMutate();
    LoadOrganelleDiff();
    LoadInterSpecies();
    LoadGroupBioFilm();
    LoadMetabolism();
    LoadEvolveStyle();
    LoadEcoEvent();
    LoadSupplyItem();
    LoadLineage();
    LoadAiBehavior();
    LoadUiTips();
    LoadChemotaxisSignal();
    LOG_INFO(MODULE_NAME, "所有配置文件加载完成");
}
//0.新增
void ConfigParser::LoadGameSetting(){
    QString path = GlobalTool::GetConfigPath() + "gameWindow.json";
    QString jsonContent = GlobalTool::ReadUtf8File(path);

    // 文件内容为空，启用兜底配置
    if (jsonContent.isEmpty())
    {
        LOG_WARN(MODULE_NAME, "gameWindow.json 内容为空，系统将使用默认格式");
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(jsonContent.toUtf8());
    if (!doc.isObject())
    {
        LOG_ERR(MODULE_NAME, "gameWindow.json 格式错误，使用默认格式");
        return;
    }
    QJsonObject obj = doc.object();
    m_gameSet.fps=obj.value("刷新频率").toInt(60);
    m_gameSet.title=obj.value("游戏名字").toString("未知");
    QJsonArray sizeArr=obj.value("窗口尺寸").toArray();
    m_gameSet.windowSize.clear();
    for(const QJsonValue& val:sizeArr){
        QSize win;
        win.setWidth(val.toObject()["宽度"].toInt(960));
        win.setHeight(val.toObject()["高度"].toInt(640));
        m_gameSet.windowSize.append(win);
    }
}

// 1. 细胞基础配置 cell_base.json
void ConfigParser::LoadCellBase()
{
    QString path = GlobalTool::GetConfigPath() + "cell_base.json";
    QString jsonContent = GlobalTool::ReadUtf8File(path);

    // 文件内容为空，启用兜底配置
    if (jsonContent.isEmpty())
    {
        LOG_WARN(MODULE_NAME, "cell_base.json 内容为空，系统将使用默认兜底配置");
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(jsonContent.toUtf8());
    // 校验JSON格式：必须是数组
    if (!doc.isArray())
    {
        LOG_ERR(MODULE_NAME, "cell_base.json 格式错误，非标准JSON数组，启用兜底配置");
        return;
    }

    QJsonArray jsonArr = doc.array();
    m_cellBaseList.clear();
    LOG_DBG(MODULE_NAME, QString("cell_base.json 解析到细胞模板数量：%1").arg(jsonArr.size()));

    // 遍历解析每个细胞模板
    for (const QJsonValue& item : jsonArr)
    {
        QJsonObject obj = item.toObject();
        Global::CellBaseProp prop;

        // 字段缺失自动使用默认值
        prop.cellID = obj["细胞编号"].toInt(1);
        prop.cellName = obj["细胞名称"].toString("本源单细胞");

        // 映射演化主干枚举
        QString mainLineText = obj["归属主干"].toString("通用");
        if (mainLineText == "裂殖演化主干")
            prop.mainLine = Global::EvolveMainLine::Fission;
        else if (mainLineText == "共生聚合主干")
            prop.mainLine = Global::EvolveMainLine::Symbiosis;
        else if (mainLineText == "代谢特化主干")
            prop.mainLine = Global::EvolveMainLine::Metabolism;
        else if (mainLineText == "古菌专属主干")
            prop.mainLine = Global::EvolveMainLine::Archaea;
        else
            prop.mainLine = Global::EvolveMainLine::Common;

        prop.initEnergy = obj["初始能量值"].toDouble(100.0);
        prop.energyPerSec = obj["每秒基础能耗"].toDouble(0.2);
        prop.moveSpeed = obj["移动速度"].toDouble(2.5);
        prop.pixelSize = obj["基础体型像素"].toInt(32);
        prop.maxDiffCount = obj["最大分化次数"].toInt(8);
        prop.toxinResist = obj["基础毒素抗性"].toInt(10);
        prop.tempResist = obj["基础高温抗性"].toInt(15);
        prop.osmoticResist = obj["基础渗透压抗性"].toInt(12);
        prop.energyWarningThresh = obj["能量预警阈值"].toDouble(20.0);
        prop.sporeDangerThresh = obj["孢子触发危险阈值"].toDouble(70.0);
        prop.sporeProb = obj["孢子触发概率(%)"].toInt(30);
        prop.energyDieImmediately = obj["能量耗尽立即凋亡"].toBool(true);

        m_cellBaseList.append(prop);
    }
}

// 2. 过渡带和物质传递规则 migrate_rule.json
void ConfigParser::LoadMigrateRule()
{
    QString path = GlobalTool::GetConfigPath() + "migrate_rule.json";
    QString jsonContent = GlobalTool::ReadUtf8File(path);
    if (jsonContent.isEmpty())
    {
        LOG_WARN(MODULE_NAME, "migrate_rule.json 内容为空，使用默认迁徙规则");
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(jsonContent.toUtf8());
    if (!doc.isObject())
    {
        LOG_ERR(MODULE_NAME, "migrate_rule.json 格式错误，使用默认迁徙规则");
        return;
    }
    QJsonObject obj = doc.object();

    m_migrateRule.nutriThresh = obj["营养匮乏阈值(%)"].toDouble(20.0);
    m_migrateRule.dangerThresh = obj["危险避险阈值(%)"].toDouble(80.0);
    m_migrateRule.densityThresh = obj["种群过载阈值(%)"].toDouble(80.0);
    m_migrateRule.moveTimeSec = obj["跨层移动耗时(秒)"].toInt(8);
    m_migrateRule.energyAddRate = obj["迁徙能耗增幅(%)"].toInt(20);
    m_migrateRule.aerobicMaxStaySec = obj["光合菌群无光层最长停留(秒)"].toInt(15);
}

// 3. 8大层级环境配置 environment_factor.json
void ConfigParser::LoadEnvFactor()
{
    QString path = GlobalTool::GetConfigPath() + "environment_factor.json";
    QString jsonContent = GlobalTool::ReadUtf8File(path);
    if (jsonContent.isEmpty())
    {
        LOG_WARN(MODULE_NAME, "environment_factor.json 内容为空，使用默认环境参数");
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(jsonContent.toUtf8());
    m_ecoLayerArr = doc.isArray() ? doc.array() : QJsonArray();
}

// 4~15 剩余配置加载（逻辑统一，异常日志规则一致）
void ConfigParser::LoadGeneMutate()
{
    QString path = GlobalTool::GetConfigPath() + "gene_mutation.json";
    QString json = GlobalTool::ReadUtf8File(path);
    if (json.isEmpty()) { LOG_WARN(MODULE_NAME, "gene_mutation.json 内容为空"); return; }
    m_mutateObj = QJsonDocument::fromJson(json.toUtf8()).object();
}

void ConfigParser::LoadOrganelleDiff()
{
    QString path = GlobalTool::GetConfigPath() + "organelle_diff.json";
    QString json = GlobalTool::ReadUtf8File(path);
    if (json.isEmpty()) { LOG_WARN(MODULE_NAME, "organelle_diff.json 内容为空"); return; }
    m_organelleObj = QJsonDocument::fromJson(json.toUtf8()).object();
}

void ConfigParser::LoadInterSpecies()
{
    QString path = GlobalTool::GetConfigPath() + "inter_species.json";
    QString json = GlobalTool::ReadUtf8File(path);
    if (json.isEmpty()) { LOG_WARN(MODULE_NAME, "inter_species.json 内容为空"); return; }
    m_interSpeciesObj = QJsonDocument::fromJson(json.toUtf8()).object();
}

void ConfigParser::LoadGroupBioFilm()
{
    QString path = GlobalTool::GetConfigPath() + "group_sense_biofilm.json";
    QString json = GlobalTool::ReadUtf8File(path);
    if (json.isEmpty()) { LOG_WARN(MODULE_NAME, "group_sense_biofilm.json 内容为空"); return; }
    m_groupBioFilmObj = QJsonDocument::fromJson(json.toUtf8()).object();
}

void ConfigParser::LoadMetabolism()
{
    QString path = GlobalTool::GetConfigPath() + "metabolism.json";
    QString json = GlobalTool::ReadUtf8File(path);
    if (json.isEmpty()) { LOG_WARN(MODULE_NAME, "metabolism.json 内容为空"); return; }
    m_metabolismObj = QJsonDocument::fromJson(json.toUtf8()).object();
}

void ConfigParser::LoadEvolveStyle()
{
    QString path = GlobalTool::GetConfigPath() + "evolution_style.json";
    QString json = GlobalTool::ReadUtf8File(path);
    if (json.isEmpty()) { LOG_WARN(MODULE_NAME, "evolution_style.json 内容为空"); return; }
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    m_evolveStyleArr = doc.isArray() ? doc.array() : QJsonArray();
}

void ConfigParser::LoadEcoEvent()
{
    QString path = GlobalTool::GetConfigPath() + "ecology_event.json";
    QString json = GlobalTool::ReadUtf8File(path);
    if (json.isEmpty()) { LOG_WARN(MODULE_NAME, "ecology_event.json 内容为空"); return; }
    m_ecoEventObj = QJsonDocument::fromJson(json.toUtf8()).object();
}

void ConfigParser::LoadSupplyItem()
{
    QString path = GlobalTool::GetConfigPath() + "supply_item.json";
    QString json = GlobalTool::ReadUtf8File(path);
    if (json.isEmpty()) { LOG_WARN(MODULE_NAME, "supply_item.json 内容为空"); return; }
    m_supplyItemObj = QJsonDocument::fromJson(json.toUtf8()).object();
}

void ConfigParser::LoadLineage()
{
    QString path = GlobalTool::GetConfigPath() + "lineage_tree.json";
    QString json = GlobalTool::ReadUtf8File(path);
    if (json.isEmpty()) { LOG_WARN(MODULE_NAME, "lineage_tree.json 内容为空"); return; }
    m_lineageObj = QJsonDocument::fromJson(json.toUtf8()).object();
}

void ConfigParser::LoadAiBehavior()
{
    QString path = GlobalTool::GetConfigPath() + "ai_behavior.json";
    QString json = GlobalTool::ReadUtf8File(path);
    if (json.isEmpty()) { LOG_WARN(MODULE_NAME, "ai_behavior.json 内容为空"); return; }
    m_aiBehaviorObj = QJsonDocument::fromJson(json.toUtf8()).object();
}

void ConfigParser::LoadUiTips()
{
    QString path = GlobalTool::GetConfigPath() + "ui_tips.json";
    QString json = GlobalTool::ReadUtf8File(path);
    if (json.isEmpty()) { LOG_WARN(MODULE_NAME, "ui_tips.json 内容为空"); return; }
    m_uiTipsObj = QJsonDocument::fromJson(json.toUtf8()).object();
}

void ConfigParser::LoadChemotaxisSignal()
{
    QString path = GlobalTool::GetConfigPath() + "chemotaxis_signal.json";
    QString json = GlobalTool::ReadUtf8File(path);
    if (json.isEmpty()) { LOG_WARN(MODULE_NAME, "chemotaxis_signal.json 内容为空"); return; }
    m_chemoSignalObj = QJsonDocument::fromJson(json.toUtf8()).object();
}

// 对外接口实现
Global::GameSetting ConfigParser::GetGameSetting() const{ return m_gameSet;}
QVector<Global::CellBaseProp> ConfigParser::GetCellBaseConfig() const { return m_cellBaseList; }
Global::MigrateRule ConfigParser::GetMigrateRuleConfig() const { return m_migrateRule; }
QJsonArray ConfigParser::GetEcoLayerConfig() const { return m_ecoLayerArr; }
QJsonObject ConfigParser::GetMutateConfig() const { return m_mutateObj; }
QJsonObject ConfigParser::GetOrganelleDiffConfig() const { return m_organelleObj; }
QJsonObject ConfigParser::GetInterSpeciesConfig() const { return m_interSpeciesObj; }
QJsonObject ConfigParser::GetGroupBioFilmConfig() const { return m_groupBioFilmObj; }
QJsonObject ConfigParser::GetMetabolismConfig() const { return m_metabolismObj; }
QJsonArray ConfigParser::GetEvolveStyleConfig() const { return m_evolveStyleArr; }
QJsonObject ConfigParser::GetEcoEventConfig() const { return m_ecoEventObj; }
QJsonObject ConfigParser::GetSupplyItemConfig() const { return m_supplyItemObj; }
QJsonObject ConfigParser::GetLineageConfig() const { return m_lineageObj; }
QJsonObject ConfigParser::GetAiBehaviorConfig() const { return m_aiBehaviorObj; }
QJsonObject ConfigParser::GetUiTipsConfig() const { return m_uiTipsObj; }
QJsonObject ConfigParser::GetChemotaxisSignalConfig() const { return m_chemoSignalObj; }