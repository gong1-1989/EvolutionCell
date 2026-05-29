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