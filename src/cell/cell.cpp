#include "Cell.h"

Cell::Cell(QObject *parent)
    : QObject(parent),
    m_uniqueId(0),
    m_currentLayer(1),
    m_groupId(-1),
    m_curEnergy(100.0),
    m_metabEff(100.0),
    m_deathRate(1.0),
    m_cellState(Global::CellState::Normal),
    m_diffCount(0),
    m_allRoundness(100.0),
    m_malignMutateCount(0),
    m_curRel(Global::InterSpeciesRel::Commensalism),
    m_relCdTime(0),
    m_groupSenseRatio(1.0),
    m_isMigrating(false),
    m_migrateTargetLayer(1),
    m_migrateStartTime(0),
    m_aerobicStayTime(0),
    m_sporeAnimFrame(0),
    m_sporeSleepStartTime(0),
    m_hasPopMigrate(false),
    m_hasPopHgt(false),
    m_hasPopBioFilm(false)
{
    // 初始兜底颜色
    m_sporeShieldColor = Qt::white;
}

Cell::~Cell()
{

}

void Cell::InitCell(int cellId, int layerId, QPointF spawnPos, const QVector<Global::Gene>& inheritGenes)
{
    // 生成全局唯一ID
    m_uniqueId = QDateTime::currentMSecsSinceEpoch() + QRandomGenerator::global()->generate();
    m_worldPos = spawnPos;
    m_inheritGeneList = inheritGenes;

    // 层级参数越界防护（合法范围 1~8）
    if ((layerId < 1) || (layerId > 8))
    {
        LOG_WARN(MODULE_NAME, QString("细胞初始层级越界(%1)，强制修正为1").arg(layerId));
        m_currentLayer = 1;
    }
    else
    {
        m_currentLayer = layerId;
    }

    // 加载细胞模板属性
    auto cellConfigList = ConfigParser::GetInstance()->GetCellBaseConfig();
    bool loadSuccess = false;
    for (const auto& prop : cellConfigList)
    {
        if (prop.cellID == cellId)
        {
            m_baseProp = prop;
            loadSuccess = true;
            break;
        }
    }

    if (!loadSuccess)
    {
        LOG_WARN(MODULE_NAME, QString("未找到ID=%1 的细胞模板，使用默认模板").arg(cellId));
    }

    // 初始化能量
    m_curEnergy = m_baseProp.initEnergy;

    // 初始拥有全部细胞器（多洛法则初始状态）
    m_organelleList = {
        Global::OrganelleType::Core_Nucleus,
        Global::OrganelleType::Core_Mitochondria,
        Global::OrganelleType::Core_Chloroplast,
        Global::OrganelleType::Sub_Flagellum,
        Global::OrganelleType::Sub_Cilium,
        Global::OrganelleType::Sub_Pseudopod
    };

    // V5.0 孢子颜色差异化配置（按演化主干区分）
    switch (m_baseProp.mainLine)
    {
    case Global::EvolveMainLine::Fission:
        m_sporeShieldColor = QColor("#F9D076");
        break;
    case Global::EvolveMainLine::Symbiosis:
        m_sporeShieldColor = QColor("#64C2E8");
        break;
    case Global::EvolveMainLine::Metabolism:
        m_sporeShieldColor = QColor("#A84A4A");
        break;
    case Global::EvolveMainLine::Archaea:
        m_sporeShieldColor = QColor("#C8D2E0");
        break;
    default:
        m_sporeShieldColor = Qt::white;
        break;
    }

    LOG_INFO(MODULE_NAME, QString("细胞[%1] 初始化完成，唯一ID：%2，所属层级：%3")
                              .arg(m_baseProp.cellName).arg(m_uniqueId).arg(m_currentLayer));
}

// ===================== AI主更新（帧率分级优化） =====================
void Cell::AIUpdate(int frameCount, bool isActive, int groupMemberCount
                    , double nicheOverlap, const Global::EnvFactor& layerEnv)
{
    // 非活跃细胞降频至15FPS，减少运算量
    if (!isActive && (frameCount % 2 != 0))
        return;

    // AI行为树：严格按优先级顺序执行
    AI_DangerDetect();
    AI_EnergyWarning();
    AI_ResponsePlayerSignal();
    CheckMigrateAction();
    AI_MigrateLogic();
    AI_EvolveLogic();
    AI_GroupLogic();
    AI_BaseAction();

    // 通用规则更新
    UpdateEnergyConsumption();
    CalcEnvironmentAdapt();
    CheckSporeSleepState();
    UpdateInterSpeciesRelation();
    UpdateGroupSense(groupMemberCount);
    CalcEcoNicheCompete(nicheOverlap);
}

// ===================== 八大生物铁律 实现 =====================
// 1. 多洛演化不可逆法则
void Cell::DiscardOrganelle(Global::OrganelleType type)
{
    m_organelleList.removeOne(type);
    // 舍弃核心细胞器，全能性大幅下降
    if (type == Global::OrganelleType::Core_Nucleus
        || type == Global::OrganelleType::Core_Mitochondria
        || type == Global::OrganelleType::Core_Chloroplast)
    {
        m_allRoundness -= 35.0;
        LOG_WARN(MODULE_NAME, QString("细胞舍弃核心细胞器，全能性下降，演化不可逆"));
    }
}

bool Cell::HasCoreOrganelle(Global::OrganelleType type) const
{
    return m_organelleList.contains(type);
}

// 2. 细胞分化法则
void Cell::DoCellDifferentiate(Global::DifferLevel level)
{
    // 分化次数超限（全局上限8次）
    if (m_diffCount >= m_baseProp.maxDiffCount)
    {
        LOG_WARN(MODULE_NAME, QString("细胞分化次数已达上限(%1)，禁止继续分化").arg(m_baseProp.maxDiffCount));
        return;
    }
    m_diffCount++;

    // 分级衰减全能性
    switch (level)
    {
    case Global::DifferLevel::Micro:
        m_allRoundness *= 0.90;
        break;
    case Global::DifferLevel::Middle:
        m_allRoundness *= 0.70;
        break;
    case Global::DifferLevel::Deep:
        m_allRoundness *= 0.25;
        DiscardOrganelle(Global::OrganelleType::Core_Nucleus);
        break;
    }
}

// 3. 基因突变规则
void Cell::DoGeneMutate(bool isStress)
{
    auto mutateCfg = ConfigParser::GetInstance()->GetMutateConfig();
    double totalProb = isStress
                           ? mutateCfg["诱导突变规则"].toObject()["单次概率(%)"].toDouble() / 100.0
                           : mutateCfg["自发突变规则"].toObject()["单次概率(%)"].toDouble() / 100.0;

    if (QRandomGenerator::global()->generateDouble() > totalProb)
        return;

    // 突变类型判定
    int randType = QRandomGenerator::global()->bounded(100);
    Global::MutateType mutType;
    if (randType < 15) mutType = Global::MutateType::Benign;
    else if (randType < 85) mutType = Global::MutateType::Neutral;
    else mutType = Global::MutateType::Malignant;

    // 恶性突变累计3次触发早衰
    if (mutType == Global::MutateType::Malignant)
    {
        m_malignMutateCount++;
        if (m_malignMutateCount >= 3)
        {
            m_baseProp.energyPerSec *= 1.5;
            LOG_WARN(MODULE_NAME, QString("细胞累计3次恶性突变，触发早衰，能耗提升50%"));
        }
    }
}

// 4. 种间关系动态法则
void Cell::UpdateInterSpeciesRelation()
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    // 冷却时间未结束，禁止切换
    if (now - m_relCdTime < Global::RELATION_CHANGE_COOLDOWN_MS)
        return;

    auto ecoArr = ConfigParser::GetInstance()->GetEcoLayerConfig();
    double nutri = ecoArr.at(m_currentLayer - 1).toObject()["平均营养浓度"].toDouble();
    double density = QRandomGenerator::global()->bounded(0, 1000)/10.0;

    if (nutri > 60.0)
        m_curRel = Global::InterSpeciesRel::Mutualism;
    else if ((nutri < 30.0) || (density > 80.0))
        m_curRel = Global::InterSpeciesRel::Antagonism;

    m_relCdTime = now;
}

// 5. 趋化优先级移动
void Cell::ChemotaxisPriorityMove(const Global::EnvFactor &layerEnv)
{
    if (m_cellState == Global::CellState::DangerEvade)
    {
        m_worldPos.rx() += m_baseProp.moveSpeed * 0.18;
        return;
    }
    if (m_cellState == Global::CellState::EnergyLack)
    {
        m_worldPos.rx() += m_baseProp.moveSpeed * 0.15;
        return;
    }
    m_worldPos.rx() += m_baseProp.moveSpeed * 0.10;
}

// 6. 群体感应法则
void Cell::UpdateGroupSense(int groupCount)
{
    if (groupCount < 10)
        m_groupSenseRatio = 1.00;
    else if (groupCount <= 50)
        m_groupSenseRatio = 1.20;
    else
        m_groupSenseRatio = 1.30;
}

// 7. 能量守恒法则
void Cell::UpdateEnergyConsumption()
{
    //【修复】每秒能耗👉转换成60FPS每帧能耗
    const double FRAME_RATE=60.0;
    double cost = m_baseProp.energyPerSec/FRAME_RATE;
    // 孢子休眠能耗减免90%
    if (m_cellState == Global::CellState::SporeSleep)
        cost *= 0.1;
    // 迁徙能耗增幅
    if (m_isMigrating)
    {
        auto migrateRule = ConfigParser::GetInstance()->GetMigrateRuleConfig();
        cost *= (1.0 + migrateRule.energyAddRate / 100.0);
    }

    m_curEnergy -= cost;
    m_curEnergy = qMax(0.0, m_curEnergy);

    // 能量归零，标记濒死
    if (m_curEnergy <= 0.0)
    {
        LOG_WARN(MODULE_NAME, QString("细胞能量耗尽，进入濒死状态，ID：%1").arg(m_uniqueId));
    }
}

// 8. 生态位竞争法则
void Cell::CalcEcoNicheCompete(double overlapRatio)
{
    if (overlapRatio > 80.0)
        m_deathRate *= 1.50;
    else if (overlapRatio > 50.0)
        m_deathRate *= 1.20;
}

// ===================== 核心业务逻辑 =====================
void Cell::CheckSporeSleepState()
{
    auto ecoArr = ConfigParser::GetInstance()->GetEcoLayerConfig();
    double danger = ecoArr.at(m_currentLayer - 1).toObject()["综合危险度"].toDouble();

    if (danger >= m_baseProp.sporeDangerThresh)
    {
        if (m_cellState != Global::CellState::SporeSleep)
        {
            if (QRandomGenerator::global()->bounded(100) < m_baseProp.sporeProb)
            {
                m_cellState = Global::CellState::SporeSleep;
                m_sporeSleepStartTime = QDateTime::currentMSecsSinceEpoch();
                m_sporeAnimFrame = 0;
                LOG_INFO(MODULE_NAME, QString("细胞进入孢子休眠状态，ID：%1").arg(m_uniqueId));
            }
        }
        else
        {
            m_sporeAnimFrame++;
            if (m_sporeAnimFrame > 7) m_sporeAnimFrame = 0;

            // 休眠超时强制凋亡
            qint64 now = QDateTime::currentMSecsSinceEpoch();
            if (now - m_sporeSleepStartTime >= Global::SPORE_MAX_SLEEP_MS)
            {
                m_curEnergy = 0;
                LOG_WARN(MODULE_NAME, QString("孢子休眠超时(120s)，细胞凋亡，ID：%1").arg(m_uniqueId));
            }
        }
    }
    else
    {
        if (m_cellState == Global::CellState::SporeSleep)
        {
            m_cellState = Global::CellState::Normal;
            LOG_INFO(MODULE_NAME, QString("环境安全，细胞退出孢子休眠，ID：%1").arg(m_uniqueId));
        }
    }
}

void Cell::CheckMigrateAction()
{
    if (m_isMigrating) return;
    auto migrateRule = ConfigParser::GetInstance()->GetMigrateRuleConfig();

    bool needMigrate = (m_curEnergy < migrateRule.nutriThresh)
                       || (m_cellState == Global::CellState::DangerEvade)
                       || (QRandomGenerator::global()->bounded(100.0) > migrateRule.densityThresh);

    if (!needMigrate) return;

    // 层级拦截规则
    if (m_baseProp.mainLine == Global::EvolveMainLine::Fission && m_currentLayer >= 5)
        return;
    if (m_currentLayer >= 8)
        return;

    // 启动迁徙
    m_isMigrating = true;
    m_migrateTargetLayer = m_currentLayer + 1;
    m_migrateStartTime = QDateTime::currentMSecsSinceEpoch();

    // 首次迁徙推送科普事件
    if (!m_hasPopMigrate)
    {
        m_hasPopMigrate = true;
        QVariantList params;
        params << m_uniqueId << m_currentLayer << m_migrateTargetLayer;
        EventBus::GetInstance()->PublishHighFreqEvent("EVT_MIGRATE", params);
    }
}

void Cell::DoHorizontalGeneTransfer()
{
    if (QRandomGenerator::global()->bounded(100) > 8) return;

    if (!m_hasPopHgt)
    {
        m_hasPopHgt = true;
        QVariantList params;
        params << m_uniqueId;
        EventBus::GetInstance()->PublishHighFreqEvent("EVT_HGT", params);
    }
}

void Cell::OperateBioFilm()
{
    auto groupCfg = ConfigParser::GetInstance()->GetGroupBioFilmConfig();
    double breakThresh = groupCfg["解体危险阈值(%)"].toDouble(75.0);
    double danger = QRandomGenerator::global()->bounded(0, 1000)/10.0;

    if (danger >= breakThresh)
    {
        if (!m_hasPopBioFilm)
        {
            m_hasPopBioFilm = true;
            QVariantList params;
            params << m_uniqueId;
            EventBus::GetInstance()->PublishHighFreqEvent("EVT_BIOFILM_BREAK", params);
        }
    }
}

void Cell::CalcEnvironmentAdapt()
{
    double miss = 100.0 - m_allRoundness;
    m_metabEff = Global::CalcMetabolismEff(100.0, miss);
    m_deathRate = Global::CalcDeathRate(1.0, miss);
}

void Cell::MoveToHighNutrition(double nutrition){

}

// ===================== AI行为树 =====================
void Cell::AI_DangerDetect()
{
    auto ecoArr = ConfigParser::GetInstance()->GetEcoLayerConfig();
    double danger = ecoArr.at(m_currentLayer - 1).toObject()["综合危险度"].toDouble();
    if (danger > 80.0 && m_cellState != Global::CellState::DangerEvade)
        m_cellState = Global::CellState::DangerEvade;
    else if (danger <= 80.0 && m_cellState == Global::CellState::DangerEvade)
        m_cellState = Global::CellState::Normal;
}

void Cell::AI_EnergyWarning()
{
    if (m_curEnergy <= m_baseProp.energyWarningThresh)
    {
        if (m_cellState == Global::CellState::Normal)
            m_cellState = Global::CellState::EnergyLack;
    }
    else
    {
        if (m_cellState == Global::CellState::EnergyLack)
            m_cellState = Global::CellState::Normal;
    }
}

void Cell::AI_ResponsePlayerSignal()
{
    // 玩家交互逻辑预留接口
}

void Cell::AI_MigrateLogic()
{
    if (!m_isMigrating) return;
    auto migrateRule = ConfigParser::GetInstance()->GetMigrateRuleConfig();
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    int totalMs = migrateRule.moveTimeSec * 1000;

    if (now - m_migrateStartTime >= totalMs)
    {
        m_currentLayer = m_migrateTargetLayer;
        m_isMigrating = false;
        m_aerobicStayTime = 0;
        return;
    }

    // 光合菌群无光层限时规则
    if (m_baseProp.mainLine == Global::EvolveMainLine::Fission && m_currentLayer >= 5)
    {
        m_aerobicStayTime += (now - m_migrateStartTime);
        if (m_aerobicStayTime >= migrateRule.aerobicMaxStaySec * 1000)
            m_migrateTargetLayer = m_currentLayer - 1;
    }
}

void Cell::AI_EvolveLogic()
{
    if (QRandomGenerator::global()->bounded(100) < 10)
        DoCellDifferentiate(Global::DifferLevel::Micro);
    DoGeneMutate(false);
    DoHorizontalGeneTransfer();
}

void Cell::AI_GroupLogic()
{
    UpdateInterSpeciesRelation();
    OperateBioFilm();
}

void Cell::AI_BaseAction()
{
    ChemotaxisPriorityMove();
    // 模拟摄食恢复能量
    if (m_curEnergy < m_baseProp.initEnergy)
        m_curEnergy += 0.01 * m_metabEff;
}

// ===================== 绘制逻辑 =====================
void Cell::Draw(QPainter* painter)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    int size = m_baseProp.pixelSize;

    // 绘制细胞本体
    painter->setBrush(Qt::white);
    painter->setPen(Qt::black);
    painter->drawEllipse(m_worldPos, size / 2, size / 2);

    // 迁徙标识图标
    if (m_isMigrating)
    {
        painter->setBrush(Qt::blue);
        painter->drawEllipse(m_worldPos + QPointF(15, -15), 8, 8);
    }

    // 孢子动画
    if (m_cellState == Global::CellState::SporeSleep)
    {
        painter->setBrush(m_sporeShieldColor);
        painter->setOpacity(0.6);
        int expand = m_sporeAnimFrame * 2;
        painter->drawEllipse(m_worldPos, size/2 + expand, size/2 + expand);
    }

    painter->restore();
}

// ===================== 对外接口实现 =====================
quint64 Cell::GetUniqueId() const { return m_uniqueId; }
Global::CellState Cell::GetCellState() const { return m_cellState; }
void Cell::SetCellState(Global::CellState state) { m_cellState = state; }
double Cell::GetCurrentEnergy() const { return m_curEnergy; }
int Cell::GetCurrentLayer() const { return m_currentLayer; }
bool Cell::IsMigrating() const { return m_isMigrating; }

bool Cell::CheckCellDeath()
{
    return m_curEnergy <= 0 || m_deathRate > 0.9;
}

bool Cell::CheckGroupDeath(int remainMember)
{
    return remainMember <= 0;
}