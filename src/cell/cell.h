#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QPointF>
#include <QDateTime>
#include <QPainter>
#include <QRandomGenerator>
#include "Global/GlobalDefine.h"
#include "Global/LogTool.h"
#include "ConfigParser/ConfigParser.h"
#include "EventBus/EventBus.h"

/**
 * @brief 细胞核心实体类 【静态内核 · 高频模块】
 * 架构说明：
 * 1. 60FPS逐帧遍历执行，底层八大生物铁律，永久静态不拆插件
 * 2. 零硬编码：所有属性、阈值、概率均从外部JSON配置读取
 * 3. 规则对标：策划第二部分八大生物铁律、第五部分生存演化、第七部分对局判定
 * 4. 日志策略：精简调试日志，仅异常/状态变更输出日志，保障运行帧率
 * 5. 异常防护：参数越界、次数超限、状态非法均做兜底+警告日志
 */
class Cell : public QObject
{
    Q_OBJECT
public:
    explicit Cell(QObject *parent = nullptr);
    ~Cell() override;

    /**
     * @brief 初始化细胞实例
     * @param cellId 细胞模板ID（对应 cell_base.json）
     * @param layerId 初始纵向层级 1~8
     * @param spawnPos 出生世界坐标
     * @param inheritGenes 上一轮轮回继承的基因列表
     */
    void InitCell(int cellId, int layerId, QPointF spawnPos, const QVector<Global::Gene>& inheritGenes);

    /**
     * @brief 细胞AI主更新函数（全局帧循环调用）
     * @param frameCount 全局帧计数器
     * @param isActive 细胞是否活跃（非活跃细胞降频至15FPS，性能优化）
     * @param groupMemberCount 所属群落总数量（群体感应/群落消亡判定）
     * @param nicheOverlap 生态位重叠度 0~100（生态竞争计算）
     * @param layerEnv 当前细胞所在层级的环境数据（上层主窗口推送）
     */
    void AIUpdate(int frameCount, bool isActive, int groupMemberCount
                  , double nicheOverlap);

    /**
     * @brief 绘制细胞本体、动画、标识（V5.0视觉优化）
     * @param painter 画布绘制器
     */
    void Draw(QPainter* painter);

    // ===================== 状态查询对外接口 =====================
    quint64 GetUniqueId() const;
    Global::CellState GetCellState() const;
    void SetCellState(Global::CellState state);
    double GetCurrentEnergy() const;
    int GetCurrentLayer() const;
    bool IsMigrating() const;

    /**
     * @brief 一级对局判定：单个细胞是否死亡
     * @return true=已死亡
     */
    bool CheckCellDeath();

    /**
     * @brief 二级对局判定：所属群落是否全部消亡
     * @param remainMember 群落剩余个体数
     * @return true=群落消亡
     */
    bool CheckGroupDeath(int remainMember);

private:
    const QString MODULE_NAME = "细胞核心";

    // ===================== 一、八大生物铁律 私有实现（策划2.1 核心规则） =====================
    /**
     * @brief 1. 多洛演化不可逆法则：舍弃核心细胞器永久无法恢复
     * @param type 细胞器类型
     */
    void DiscardOrganelle(Global::OrganelleType type);
    bool HasCoreOrganelle(Global::OrganelleType type) const;

    /**
     * @brief 2. 细胞分化法则：单局最大分化8次，深度特化不可逆
     * @param level 分化等级
     */
    void DoCellDifferentiate(Global::DifferLevel level);

    /**
     * @brief 3. 基因突变规则：自发/诱导突变，累计3次恶性突变触发细胞早衰
     * @param isStress true=环境诱导突变  false=自发突变
     */
    void DoGeneMutate(bool isStress);

    /**
     * @brief 4. 种间关系动态法则：切换冷却固定20秒
     */
    void UpdateInterSpeciesRelation();

    /**
     * @brief 5. 微生物趋化优先级法则：避险 > 觅食 > 常规移动
     */
    void ChemotaxisPriorityMove();

    /**
     * @brief 6. 群体感应法则：根据群落数量获得协同增益
     * @param groupCount 群落总个体数
     */
    void UpdateGroupSense(int groupCount);

    /**
     * @brief 7. 能量守恒法则：基础能耗 + 迁徙增幅 + 孢子休眠减免
     */
    void UpdateEnergyConsumption();

    /**
     * @brief 8. 生态位竞争法则：重叠度越高，死亡率增幅越大
     * @param overlapRatio 生态位重叠百分比 0~100
     */
    void CalcEcoNicheCompete(double overlapRatio);

    // ===================== 二、核心业务逻辑（生存/演化/迁徙/孢子） =====================
    /**
     * @brief 孢子休眠状态逻辑（V5.0体验优化3 + 策划5.1.2）
     * 规则：危险达标概率触发，最长休眠120秒，超时自动凋亡
     */
    void CheckSporeSleepState();

    /**
     * @brief 跨层迁徙逻辑（V5.0体验优化1 + 策划3.5）
     * 规则：三大触发条件、层级拦截、光合菌群限时、固定耗时
     */
    void CheckMigrateAction();

    /**
     * @brief 水平基因转移HGT（V5.0体验优化4）
     * 8%触发概率，单局首次触发推送科普事件
     */
    void DoHorizontalGeneTransfer();

    /**
     * @brief 生物膜构筑/解体（V5.0体验优化4）
     * 环境危险超标触发解体，单局首次触发推送科普事件
     */
    void OperateBioFilm();

    /**
     * @brief 环境适配计算（全局固定公式）
     * 计算代谢效率、实际死亡率
     */
    void CalcEnvironmentAdapt();

    /**
     * @brief 觅食移动辅助函数
     * @param nutrition 能量密度
     */
    void MoveToHighNutrition(double nutrition);

    // ===================== 三、AI行为树（固定优先级：从上至下执行） =====================
    void AI_DangerDetect();        // 1.最高优先级：危险检测与避险
    void AI_EnergyWarning();       // 2.次优先级：能量预警与觅食
    void AI_ResponsePlayerSignal();// 3.响应玩家交互引导
    void AI_MigrateLogic();        // 4.跨层迁徙状态更新
    void AI_EvolveLogic();         // 5.演化逻辑：分化 + 突变 + 基因转移
    void AI_GroupLogic();          // 6.群落逻辑：种间关系 + 生物膜
    void AI_BaseAction();          // 7.最低优先级：基础移动 + 摄食

private:
    // 基础标识
    quint64                 m_uniqueId;             // 细胞全局唯一ID
    Global::CellBaseProp    m_baseProp;              // 细胞基础属性（JSON加载）
    QPointF                 m_worldPos;             // 世界坐标
    int                     m_currentLayer;         // 当前层级 1~8
    int                     m_groupId;              // 所属群落ID

    // 生存属性
    double                  m_curEnergy;            // 当前能量值
    double                  m_metabEff;             // 当前代谢效率
    double                  m_deathRate;            // 当前死亡率
    Global::CellState       m_cellState;            // AI状态机

    // 演化属性
    int                     m_diffCount;            // 已分化次数（上限8）
    double                  m_allRoundness;         // 全能性 0~100%
    int                     m_malignMutateCount;    // 恶性突变累计次数（上限3）
    QVector<Global::OrganelleType> m_organelleList; // 现有细胞器列表

    // 种间关系 & 群体感应
    Global::InterSpeciesRel m_curRel;               // 当前种间关系
    qint64                  m_relCdTime;             // 关系切换冷却时间戳
    double                  m_groupSenseRatio;      // 群体感应协同系数

    // 跨层迁徙
    bool                    m_isMigrating;          // 是否正在迁徙
    int                     m_migrateTargetLayer;   // 迁徙目标层级
    qint64                  m_migrateStartTime;     // 迁徙开始时间戳
    qint64                  m_aerobicStayTime;      // 光合菌群无光层停留时长

    // 孢子动画（三大主干差异化配色）
    int                     m_sporeAnimFrame;       // 孢子动画帧 0~6
    QColor                  m_sporeShieldColor;     // 孢子护盾颜色
    qint64                  m_sporeSleepStartTime;  // 孢子休眠开始时间

    // 科普弹窗标记（单局仅首次触发）
    bool                    m_hasPopMigrate;
    bool                    m_hasPopHgt;
    bool                    m_hasPopBioFilm;

    // 基因谱系
    QVector<Global::Gene>  m_inheritGeneList;       // 轮回继承基因
};

#endif // CELL_H