#ifndef GLOBALDEFINE_H
#define GLOBALDEFINE_H

#include <QString>
#include <QVector>
#include <QPointF>
#include <QJsonObject>
#include <QDateTime>
// 引入日志模块，全局所有模块共用日志
#include "LogTool.h"

/**
 * @brief 全局定义模块 【静态内核】
 * 架构说明：底层枚举、数据结构体、数学公式、全局常量
 * 1. 全项目强依赖，永久不拆分为DLL插件
 * 2. 正式版本发布后，结构体字段禁止删除/修改，仅允许在尾部追加字段
 * 3. 所有规则严格对标《进化亿重奏：细胞纪元 V5.0》策划书
 */
namespace Global
{
// ===================== 一、基础枚举体系（对标策划全规则） =====================
/**
     * @brief 演化主干枚举：四大演化方向 + 通用本源细胞 + 古菌DLC分支,对应 cell_base.json 配置
     */
enum class EvolveMainLine : int
{
    Common = 0,      // 本源单细胞 - 通用主干
    Fission = 1,     // 裂殖演化主干（光合菌群）
    Symbiosis = 2,   // 共生聚合主干
    Metabolism = 3,  // 代谢特化主干
    Archaea = 4      // 古菌专属主干（DLC拓展内容）
};

/**
     * @brief 细胞分化等级：遵循分化不可逆法则,对应 organelle_diff.json 配置
     */
enum class DifferLevel : int
{
    Micro = 0,       // 微分化（全能性衰减少）
    Middle = 1,      // 中度分化
    Deep = 2         // 深度特化（不可逆，舍弃核心细胞器）
};

/**
     * @brief 基因突变类型：累计3次恶性突变会触发细胞早衰,对应 gene_mutation.json 配置
     */
enum class MutateType : int
{
    Benign = 0,      // 良性突变（增益属性）
    Neutral = 1,     // 中性突变（无影响）
    Malignant = 2    // 恶性突变（损耗属性、触发早衰）
};

/**
     * @brief 细胞AI状态机（执行优先级：避险 > 觅食 > 正常 > 孢子休眠）
     */
enum class CellState : int
{
    Normal = 0,      // 正常状态
    EnergyLack = 1,  // 能量匮乏（优先执行觅食行为）
    DangerEvade = 2, // 危险避险（最高优先级行为）
    SporeSleep = 3   // 孢子休眠（能耗降低90%）
};

/**
     * @brief 玩家三层交互模式（对应交互权重规则）
     * 权重：旁观0% | 趋化引导40% | 深度干预60% ,对应 chemotaxis_signal.json 配置
     */
enum class InteractMode : int
{
    Watch = 1,       // 旁观模式
    Chemotaxis = 2,  // 趋化引导模式
    DeepInterfere = 3// 深度干预模式
};

/**
     * @brief 生态演替四阶段：生态整体不可逆演化,对应 environment_factor.json 配置
     */
enum class EcoStage : int
{
    Early = 0,   // 生态初期（营养充足）
    Middle = 1,  // 生态中期
    Late = 2,    // 生态后期（营养匮乏）
    Final = 3    // 生态末期（全域濒临崩溃）
};

/**
     * @brief 四级对局结束判定（从单体死亡到全域生态灭亡）
     */
enum class GameOverLevel : int
{
    CellDeath = 0,    // 一级：单个细胞死亡（对局继续）
    GroupDeath = 1,   // 二级：整个菌群消亡（对局继续）
    HabitatCrash = 2, // 三级：局部生态层崩溃（对局继续）
    EcoTotalEnd = 3   // 四级：全域生态灭亡（本局强制终止）
};

/**
     * @brief V5.0科普弹窗触发类型
     */
enum class PopScienceType : int
{
    Migrate = 0,      // 跨层迁徙科普
    BioFilmBreak = 1, // 生物膜解体科普
    HGT = 2           // 水平基因转移科普
};

/**
     * @brief 六大种间关系：切换冷却时间固定20秒,对应 inter_species.json 配置
     */
enum class InterSpeciesRel : int
{
    Commensalism = 0,    // 共栖
    Mutualism = 1,       // 互利共生
    ParasiteLight = 2,   // 偏利寄生
    ParasiteStrict = 3,  // 专性寄生
    Antagonism = 4,      // 菌群对抗
    SaprophyticSym = 5   // 腐生共生
};

/**
     * @brief 细胞器类型：核心细胞器舍弃后永久无法恢复（多洛不可逆法则）,对应 organelle_diff.json 配置
     */
enum class OrganelleType : int
{
    Core_Nucleus = 0,        // 细胞核【核心细胞器】
    Core_Mitochondria = 1,   // 线粒体【核心细胞器】
    Core_Chloroplast = 2,    // 叶绿体【核心细胞器】
    Sub_Flagellum = 3,       // 鞭毛【次要细胞器】
    Sub_Cilium = 4,          // 纤毛【次要细胞器】
    Sub_Pseudopod = 5        // 伪足【次要细胞器】
};

// ===================== 二、核心数据结构体（映射外部JSON配置） =====================
/**
     * @brief 细胞基础属性：完全映射 cell_base.json 配置文件
     */
struct CellBaseProp
{
    int cellID;                  // 细胞模板编号
    QString cellName;            // 细胞名称
    EvolveMainLine mainLine;     // 所属演化主干
    double initEnergy;           // 初始能量
    double energyPerSec;         // 每秒基础能耗
    double moveSpeed;            // 移动速度
    int pixelSize;               // 绘制像素尺寸
    int maxDiffCount;            // 单局最大分化次数（全局上限8）
    int toxinResist;             // 毒素抗性
    int tempResist;              // 高温抗性
    int osmoticResist;           // 渗透压抗性
    double energyWarningThresh;  // 能量预警阈值
    double sporeDangerThresh;    // 触发孢子休眠的危险阈值
    int sporeProb;               // 孢子触发概率 %
    bool energyDieImmediately;   // 能量耗尽是否立即凋亡

    CellBaseProp() : cellID(0), mainLine(EvolveMainLine::Common) {}
};

/**
 * @brief 游玩模式:完全映射 difficulty_mode.json 配置文件
 */
struct diffMode
{
    QString name;                   // 模式名称
    int mutationDecrease;           // 突变概率降幅(%)
    qreal rateCoefficient;          // 演替速度系数
    int negative;                   // 负面数值增幅(%)
    bool switching;                 // 禁止跨主干切换
    bool modificationEnvironment;   // 允许手动修改环境参数
    diffMode():mutationDecrease(0),rateCoefficient(1.0)
        ,negative(0),switching(false),modificationEnvironment(false){}
};

/**
     * @brief 环境六因子：8大纵向生态层级通用环境参数,对应 environment_layer.json 配置
     * 单位：温度(℃)、pH值、其余参数为百分比(0~100)
     */
struct EnvFactor
{
    int id;                         // 层级编号
    QString name;                   // 层级名称
    int dangerLevel;                // 危险等级
    qint64 successionCycle ;        // 演替周期(秒)
    qreal rangeNum;                 // 数值波动幅度(%)
    double minTemp;                 // 温度区间-最小
    double maxTemp;                 // 温度区间-最大
    double minpH;                   // pH值区间-最小
    double maxpH;                   // pH值区间-最大
    double minOxygen;               // 溶氧量区间(%)-最小
    double maxOxygen;               // 溶氧量区间(%)-最大
    double minOsmotic;              // 渗透压区间(%)-最小
    double maxOsmotic;              // 渗透压区间(%)-最大
    double minToxin;                // 毒素浓度区间(%)-最小
    double maxToxin;                // 毒素浓度区间(%)-最大
    double minNutrition;            // 营养浓度区间(%)-最小
    double maxNutrition;            // 营养浓度区间(%)-最大

    // 构造函数：初始化兜底默认值
    EnvFactor() : minTemp(20),maxTemp(28), minpH(6.5),maxpH(7.5), minOxygen(80),maxOxygen(95)
        , minOsmotic(10),maxOsmotic(20), minToxin(0),maxToxin(10), minNutrition(70), maxNutrition(90) {}
};

/**
 * @brief T演化流派配置:完全映射 evolution_style.json 配置文件
 */
struct Sect{
    int ID;                             // 流派ID
    QString name;                       // 流派名称
    EvolveMainLine mainLine;            // 归属主干
    QString color;                      // 主色调
    QString sporeAnimationType;         // 孢子动画类型
    QVector<QJsonObject> attribute;     // 核心属性
    QVector<QJsonArray> stratum;        // 适配层级
    QVector<QJsonArray> restrained;     // 克制流派
    QVector<QJsonArray> beRestrained;   // 被克制流派
};
/**
 * @brief 游戏通用设置：完全映射 gemasetting.json 配置文件
 */
struct GameSetting{
    QJsonObject windowSize;             // 预设的窗口尺寸
    QString title;                      // 游戏名字
    int fps;                            // 刷新频率
};

/**
     * @brief 基因结构体：轮回继承规则:完全映射 gene_library.json 配置文件
     * 规则：显性基因永久继承 | 隐性基因保留3轮 | 缺陷基因禁止继承
     */
struct Gene
{
    int geneID;                         // 基因唯一编号
    QString geneName;                   // 基因名称
    QString geneType;                   // 基因类型：显性/隐性/缺陷
    QString classify;                   // 性状分类
    QString description;                // 性状效果描述
    QJsonObject effectValue;            // 基因增益/减益数值
    QString source;                     // 来源
    int keepRound;                      // 剩余存续轮数
    QVector<EvolveMainLine> mainList;   // 适配主干

    Gene() : geneID(0), keepRound(0) {}
};

/**
 * @brief 关系类型:完全映射 inter_species.json 配置文件中的 "关系类型列表"
 */
struct InterSpecies{
    InterSpeciesRel name;               // 关系名称
    qreal sufficient;                   // 营养充足稳定性(%)
    qreal scarcity;                     // 营养匮乏稳定性(%)
    qreal densityCoefficient;           // 种群密度影响系数
};

// ===================== 三、全局固定计算公式（底层铁律，禁止修改） =====================
/**
     * @brief 计算实际代谢效率
     * 公式：实际代谢效率 = 基准效率 × (1 - 环境缺失度 × 0.15)
     */
inline double CalcMetabolismEff(double baseEff, double missDegree)
{
    return baseEff * (1.0 - missDegree * 0.15);
}

/**
     * @brief 计算实际死亡率
     * 公式：实际死亡率 = 基准死亡率 × (1 + 环境缺失度 × 0.08)
     */
inline double CalcDeathRate(double baseRate, double missDegree)
{
    return baseRate * (1.0 + missDegree * 0.08);
}

// ===================== 四、全局常量（JSON读取失败时的兜底阈值） =====================
constexpr int AI_FPS_ACTIVE = 30;         // 活跃细胞AI帧率
constexpr int AI_FPS_INACTIVE = 15;       // 非活跃细胞AI降频帧率
constexpr int POP_W = 200;               // 科普弹窗宽度
constexpr int POP_H = 80;                // 科普弹窗高度
constexpr int POP_SHOW_MS = 5000;        // 科普弹窗展示时长 毫秒
constexpr int WARN_LATE_INTERVAL_MS = 30000;  // 生态后期预警间隔 30秒
constexpr int WARN_FINAL_INTERVAL_MS = 60000; // 生态末期预警间隔 60秒
constexpr int SPORE_MAX_SLEEP_MS = 120000;    // 孢子最大休眠时长 120秒
constexpr int RELATION_CHANGE_COOLDOWN_MS = 20000; // 种间关系切换冷却 20秒
constexpr int ARCHAEA_FLUCT_CYCLE_MS = 200000;    // 古菌层波动周期 200秒
constexpr double ARCHAEA_FLUCT_RATIO = 8.0;       // 古菌层波动幅度 8%
}

#endif // GLOBALDEFINE_H


#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <QString>
#include <QList>
#include <QMap>
#include <QColor>

// ====================== 强类型枚举（对应JSON枚举值） ======================
namespace GlobalEnum
{
// 基因类型
enum class GeneType {
    Dominant,     // 显性
    Recessive,    // 隐性
    Defective     // 缺陷
};

// 性状分类
enum class TraitType {
    Proliferate,  // 增殖
    Defense,      // 防御
    StressResist, // 抗逆
    Metabolism,   // 代谢
    Mimicry,      // 拟态
    Toxin,        // 毒素
    Symbiosis     // 共生
};

// 危险类型
enum class DangerType {
    ToxinErosion,   // 毒素侵蚀
    Predation,      // 天敌捕食
    ExtremeEnv,     // 极端环境
    NutritionLack   // 营养匮乏
};

// 分化等级
enum class DifferLevel {
    Micro,      // 微分化
    Medium,     // 中度分化
    DeepSpecial // 深度特化
};

// 游玩模式
enum class PlayMode {
    Novice,     // 循序演化模式(新手)
    Standard,   // 无尽生态模式(标准)
    Trial,      // 流派试炼模式
    Sandbox     // 自由沙盒模式(硬核)
};
}

// ====================== 全局配置命名空间（核心） ======================
namespace GlobalConfig
{
// 元信息
const QString META_NAME = "细胞演化模拟器";
const QString META_VERSION = "1.0";
const QString META_DESC = "全局配置定义模块";

//==========================================================================
// 1. global_rule.json → 全局基础规则
//==========================================================================
struct GeneralFormula {
    QString metabolismEfficiency; // 实际代谢效率公式
    QString mortalityRate;        // 实际死亡率公式
};
const GeneralFormula GENERAL_FORMULA = {
    "基准效率 × (1 - 匹配缺失度 × 0.15)",
    "基准死亡率 × (1 + 匹配缺失度 × 0.08)"
};

// 危险优先级排序
const QList<GlobalEnum::DangerType> DANGER_PRIORITY = {
    GlobalEnum::DangerType::ToxinErosion,
    GlobalEnum::DangerType::Predation,
    GlobalEnum::DangerType::ExtremeEnv,
    GlobalEnum::DangerType::NutritionLack
};

// 拟态规则
struct MimicryRule {
    int lowSimilarityThreshold;    // 低相似度识破阈值(%)
    int lowSimilarityRate;         // 低相似度识破率(%)
    int highSimilarityThreshold;   // 高相似度识破阈值(%)
    int highSimilarityRate;        // 高相似度识破率(%)
    int visibilityReduceRate;      // 能见度每降10%识破率扣减(%)
};
const MimicryRule MIMICRY_RULE = {40,80,80,10,12};

// 营养切换规则
const int NUTRITION_SWITCH_COOL = 5; // 秒

// 数据留存规则
struct DataSaveRule {
    bool dominantGenePermanent;
    int recessiveGeneRound;
    bool defectiveGeneInherit;
};
const DataSaveRule DATA_SAVE_RULE = {true, 3, false};

//==========================================================================
// 2. ai_runtime.json → AI运行参数
//==========================================================================
struct AIFps {
    int active;
    int inactive;
};
const AIFps AI_FPS = {30,15};

// 迁徙判定阈值
struct MigrateThreshold {
    int nutritionLack;    // 营养匮乏迁徙阈值(%)
    int dangerAvoid;      // 危险避险迁徙阈值(%)
    int populationOverload;// 种群过载迁徙阈值(%)
    int crossLayerTime;   // 跨层移动耗时(秒)
    int crossLayerCost;   // 跨层能耗增幅(%)
};
const MigrateThreshold MIGRATE_THRESHOLD = {20,80,80,8,20};

// 凋亡&崩溃阈值
struct ApoptosisThreshold {
    int toxinApoptosisTime;   // 毒素凋亡累计时长(秒)
    int extremeEnvDeath;      // 极端环境致死阈值(%)
    int groupCollapseMin;     // 群落溃散最小数量
    int habitatCollapseNutri; // 生境崩溃营养阈值(%)
    int habitatCollapseTime;  // 生境崩溃持续时长(秒)
    int sporeMaxDormancy;     // 孢子蛰伏最长时长(秒)
};
const ApoptosisThreshold APOPTOSIS_THRESHOLD = {10,90,2,10,30,120};

// AI容错规则
struct AIFaultTolerant {
    int targetLostTimeout; // 目标丢失搜寻超时(秒)
    int groupCollapseMin;  // 群落溃散种群下限
    bool loadDefaultOnFail;// 配置读取失败加载默认值
};
const AIFaultTolerant AI_FAULT_TOLERANT = {3,5,true};

//==========================================================================
// 3. lineage_save.json → 谱系存档规则
//==========================================================================
const int LOCAL_SAVE_MAX_ROUND = 50;    // 本地存档最大轮数
const int SAVE_SLOT_COUNT = 5;           // 独立存档槽位数量
const QString SAVE_ENCRYPT_TYPE = "简易JSON加密";

// 生存时长奖励梯度
struct SurvivalReward {
    int minTime;
    int maxTime;
    int nutriBonus;
};
const QList<SurvivalReward> SURVIVAL_REWARD_LIST = {
    {0,300,0}, {301,900,10}, {901,1800,25}, {1801,9999,40}
};

//==========================================================================
// 4. difficulty_mode.json → 难度模式
//==========================================================================
const QMap<QString, int> PLAYER_COMMAND_WEIGHT = {
    {"旁观模式",0}, {"引导模式",40}, {"深度模式",60}
};

// 游玩模式参数
struct DifficultyParam {
    GlobalEnum::PlayMode mode;
    int mutationReduce;      // 突变概率降幅(%)
    double evolveSpeed;      // 演替速度系数
    int negativeValueAdd;    // 负面数值增幅(%)
    bool banSwitchMain;      // 禁止跨主干切换
    bool allowEditEnv;       // 允许手动修改环境参数
};
const QList<DifficultyParam> DIFFICULTY_LIST = {
    {GlobalEnum::PlayMode::Novice,    30, 0.7, -25, false, false},
    {GlobalEnum::PlayMode::Standard,  0,  1.0, 0,    false, false},
    {GlobalEnum::PlayMode::Trial,     0,  1.0, 15,   true,  false},
    {GlobalEnum::PlayMode::Sandbox,   0,  1.0, 15,   false, true}
};

//==========================================================================
// 5. cell_base.json → 细胞基础属性
//==========================================================================
struct CellBase {
    int id;                 // 细胞编号
    QString name;            // 细胞名称
    QString mainType;       // 归属主干
    int initEnergy;         // 初始能量值
    double costPerSecond;   // 每秒基础能耗
    double moveSpeed;       // 移动速度
    int sizePixel;          // 基础体型像素
    int maxDiffCount;       // 最大分化次数
    int toxinResist;        // 基础毒素抗性
    int highTempResist;     // 基础高温抗性
    int osmoticResist;      // 基础渗透压抗性
    int energyWarn;         // 能量预警阈值
    int sporeDangerThreshold;// 孢子触发危险阈值
    int sporeProbability;   // 孢子触发概率(%)
    bool apoptosisOnEnergyEmpty;// 能量耗尽立即凋亡
    QString desc;           // 描述
};
const QList<CellBase> CELL_BASE_LIST = {
    {1,"本源单细胞","通用",100,0.2,2.5,32,8,10,15,12,20,70,30,true,"游戏初始全能细胞"},
    {2,"基础裂殖细胞","裂殖演化主干",90,0.22,2.2,32,8,8,20,18,20,65,50,true,"擅长分裂增殖"},
    {3,"共生菌群细胞","共生聚合主干",105,0.18,2.0,48,8,15,12,14,20,75,30,true,"擅长群落协作"},
    {4,"代谢特化细胞","代谢特化主干",95,0.25,2.3,32,8,22,25,24,20,72,25,true,"环境适应能力极强"}
};

//==========================================================================
// 6. organelle_diff.json → 细胞器分化
//==========================================================================
struct Organelle {
    QString name;
    int penaltyValue;
    bool irreversible;
};
const QList<Organelle> CORE_ORGANELLE = {{"细胞核",40,true},{"线粒体",35,true},{"叶绿体",30,true}};
const QList<Organelle> MINOR_ORGANELLE = {{"鞭毛",30,false},{"纤毛",20,false},{"伪足",25,false}};

// 分化规则
struct DifferRule {
    GlobalEnum::DifferLevel level;
    int totipotencyRetain;  // 全能性保留比例(%)
    int energyCost;         // 单次能耗消耗
    QString allowAdjust;    // 允许调整细胞器
    bool irreversible;      // 是否触发演化不可逆
};
const QList<DifferRule> DIFFER_RULE_LIST = {
    {GlobalEnum::DifferLevel::Micro,90,10,"仅次要细胞器",false},
    {GlobalEnum::DifferLevel::Medium,70,25,"全部细胞器",false},
    {GlobalEnum::DifferLevel::DeepSpecial,25,40,"可舍弃核心细胞器",true}
};

//==========================================================================
// 8. gene_mutation.json → 基因突变规则
//==========================================================================
struct MutationRule {
    double probability; // 触发概率(%)
    int goodRate;       // 良性占比(%)
    int neutralRate;    // 中性占比(%)
    int badRate;        // 恶性占比(%)
};
const MutationRule SPONTANEOUS_MUTATION = {0.05,15,70,15};
const MutationRule INDUCED_MUTATION = {20,25,50,25};

//==========================================================================
// 12. metabolism.json → 代谢通路
//==========================================================================
struct MetabolismPath {
    int normalEfficiency;    // 常规环境效率(%)
    int specialEfficiency;   // 特殊环境效率(%)
    double costCoefficient;  // 基础能耗系数
};
const MetabolismPath METABOLISM_AEROBIC = {100,30,1.0};
const MetabolismPath METABOLISM_ANAEROBIC = {60,90,1.2};
const MetabolismPath METABOLISM_CHEMOSYNTHESIS = {80,40,1.8};

//==========================================================================
// 19. visual_effect.json → 视觉效果
//==========================================================================
struct SporeAnimation {
    QString form;
    double shrinkTime;
    QColor shieldColor;
    QString blinkFreq;
};
const QMap<QString, SporeAnimation> SPORE_ANIMATION_CONFIG = {
    {"裂殖型", {"圆形紧凑",0.4,QColor("#F9D076"),"偏高"}},
    {"共生型", {"椭圆形",0.6,QColor("#64C2E8"),"平缓"}},
    {"代谢型", {"不规则小型",0.5,QColor("#A84A4A"),"中等"}}
};

}

#endif // GLOBALCONFIG_H