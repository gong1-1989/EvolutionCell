#include "EcologyCore.h"

// 单例静态初始化
EcologyCore* EcologyCore::m_instance = nullptr;

EcologyCore* EcologyCore::GetInstance()
{
    static QMutex instMutex;
    QMutexLocker locker(&instMutex);

    if (m_instance == nullptr)
    {
        m_instance = new EcologyCore;
        LOG_INFO(m_instance->MODULE_NAME, "生态核心单例对象创建成功");
    }
    return m_instance;
}

void EcologyCore::InitEcology()
{
    // 下标0预留，1~8对应8个生态层级
    m_layerEnvList.resize(9);
    // 初始演替阶段：生态初期
    m_globalEcoStage = Global::EcoStage::Early;
    // 初始化古菌波动计时
    m_archaeaFluctTime = QDateTime::currentMSecsSinceEpoch();

    // 从JSON配置加载初始环境参数
    QJsonArray layerConfigArr = ConfigParser::GetInstance()->GetEcoLayerConfig();
    int loadCount = 0;

    for (int i = 0; i < 8 && i < layerConfigArr.size(); ++i)
    {
        QJsonObject obj = layerConfigArr.at(i).toObject();
        int layerIndex = i + 1;
        Global::EnvFactor& env = m_layerEnvList[layerIndex];

        // 读取配置，字段缺失使用结构体默认值
        env.minTemp = obj["温度下限"].toDouble(env.minTemp);
        env.maxTemp = obj["温度上限"].toDouble(env.maxTemp);
        env.temp=Global::randomDouble(env.minTemp,env.maxTemp);
        env.minpH = obj["pH下限"].toDouble(env.minpH);
        env.maxpH = obj["pH上限"].toDouble(env.maxpH);
        env.pH=Global::randomDouble(env.minpH,env.maxpH);
        env.minOxygen = obj["溶氧量下限(%)"].toDouble(env.minOxygen);
        env.maxOxygen = obj["溶氧量上限(%)"].toDouble(env.maxOxygen);
        env.oxygen=Global::randomDouble(env.minOxygen,env.maxOxygen);
        env.minOsmotic = obj["渗透压下限(%)"].toDouble(env.minOsmotic);
        env.maxOsmotic = obj["渗透压上限(%)"].toDouble(env.maxOsmotic);
        env.osmotic=Global::randomDouble(env.minOsmotic,env.maxOsmotic);
        env.minToxin = obj["毒素下限(%)"].toDouble(env.minToxin);
        env.maxToxin = obj["毒素上限(%)"].toDouble(env.maxToxin);
        env.toxin=Global::randomDouble(env.minToxin,env.maxToxin);
        env.minNutrition = obj["营养下限(%)"].toDouble(env.minNutrition);
        env.maxNutrition = obj["营养上限(%)"].toDouble(env.maxNutrition);
        env.nutrition=Global::randomDouble(env.minNutrition,env.maxNutrition);
        //横向地貌修正所在层级环境
        CalcLandformEffect(i);
        // 初始截断数值，保证合规
        ClampEnvValue(env,i);
        loadCount++;
    }

    LOG_INFO(MODULE_NAME, QString("8层生态环境初始化完成，成功加载配置层数：%1").arg(loadCount));
}

void EcologyCore::EcologyUpdate(int frameCount)
{
    Q_UNUSED(frameCount);

    // 1. 自然衰减：营养、毒素、氧气每帧?缓慢下降0.01
    for (int layer = 1; layer <= 8; ++layer)
    {
        Global::EnvFactor& env=m_layerEnvList[layer];
        const double decayRate=0.99;
        env.nutrition*=decayRate;
        env.oxygen*=decayRate;
        env.toxin*=decayRate;
    }

    // 2. 层间物质自上而下传导
    TransmitBetweenLayer();

    // 3. 古菌层（第8层）周期性波动
    ArchaeaLayerFluctuate();

    // 4. 第一层气水界面专属加成
    CalcInterfaceBonus();

    // 5. 更新全域生态演替阶段
    UpdateEcoSuccessionStage();
}

Global::EnvFactor EcologyCore::GetLayerEnvironment(int layerId) const
{
    // 层级合法性校验：仅允许 1~8
    if ((layerId < 1) || (layerId > 8))
    {
        LOG_WARN(MODULE_NAME, QString("查询非法层级(%1)，合法范围1~8，返回默认环境参数").arg(layerId));
        return Global::EnvFactor();
    }
    return m_layerEnvList[layerId];
}

Global::EcoStage EcologyCore::GetGlobalEcoStage() const
{
    return m_globalEcoStage;
}

void EcologyCore::CalcLandformEffect(int layerId)
{
    // 随机选取6类横向地貌，对环境进行随机3.0~8.0小幅修正
    double_t randMod=Global::randomDouble(3,8);
    int landType = QRandomGenerator::global()->bounded(0, 6);
    Global::EnvFactor& env = m_layerEnvList[layerId];

    switch (landType)
    {
    case 0: env.nutrition += randMod; break;    // 富营养地貌
    case 1: env.temp += randMod; break;        // 高温地貌
    case 2: env.oxygen -= randMod; break;      // 低氧地貌
    case 3: env.toxin += randMod; break;       // 有毒地貌
    case 4: env.osmotic += randMod; break;     // 高渗透压地貌
    case 5: break;                         // 普通地貌，无修正
    }
}

void EcologyCore::TransmitBetweenLayer()
{
    // 规则：物质从上层向下层传导，传导比例10%
    const double transmitRate = 0.1/60.0;
    // 规则： 每层多消耗5%的物质
    const double consumeRate=0.05/60.0;
    // 从顶层1向底层8倒序遍历
    for (int layer = 1; layer <=8; ++layer)
    {
        Global::EnvFactor& currLayer = m_layerEnvList[layer];
        Global::EnvFactor& upLayer = m_layerEnvList[layer - 1];
        //本层营养浓度＞本层上限 && 本层营养浓度＞下层浓度才向下传导
        if(currLayer.nutrition>currLayer.maxNutrition && currLayer.nutrition>upLayer.nutrition){
            upLayer.nutrition += currLayer.nutrition * transmitRate;
            currLayer.nutrition*=(1-consumeRate-transmitRate);
        }
        //本层毒素浓度＞本层上限 && 本层毒素浓度＞下层浓度才向下传导
        if(currLayer.toxin>currLayer.maxToxin && currLayer.toxin>upLayer.toxin){
            upLayer.toxin += currLayer.toxin * transmitRate;
            currLayer.toxin*=(1-consumeRate-transmitRate);
        }
        //本层氧气浓度＞本层上限 && 本层氧气浓度＞下层浓度才向下传导
        if(currLayer.oxygen>currLayer.maxOxygen && currLayer.oxygen>upLayer.oxygen){
            upLayer.oxygen += currLayer.oxygen * transmitRate;
            currLayer.oxygen*=(1-consumeRate-transmitRate);
        }
    }
}

void EcologyCore::ArchaeaLayerFluctuate()
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    // 未到波动周期，直接返回
    if (now - m_archaeaFluctTime < Global::ARCHAEA_FLUCT_CYCLE_MS)
        return;

    // 更新波动时间戳
    m_archaeaFluctTime = now;
    double fluctRatio = Global::ARCHAEA_FLUCT_RATIO / 100.0;
    // 随机正负波动
    if (QRandomGenerator::global()->bounded(2))
        fluctRatio = -fluctRatio;

    // 仅古菌层（第8层）温度、渗透压、毒素产生波动
    Global::EnvFactor& archaeaEnv = m_layerEnvList[8];
    archaeaEnv.temp *= (1 + fluctRatio);
    archaeaEnv.osmotic *= (1 + fluctRatio);
    archaeaEnv.toxin *= (1 + fluctRatio);

    // 数值截断并输出日志
    ClampEnvValue(archaeaEnv,8);
    LOG_DBG(MODULE_NAME, "古菌层触发周期性环境波动");
}

void EcologyCore::CalcInterfaceBonus()
{
    // 第一层为气-水交界面，营养、溶氧量获得固定加成
    Global::EnvFactor& layer1 = m_layerEnvList[1];
    layer1.nutrition *= (1.0+0.1/60.0);
    layer1.oxygen *= (1.0+0.15/60.0);
    ClampEnvValue(layer1,1);
}

void EcologyCore::UpdateEcoSuccessionStage()
{
    // 计算8个层级平均营养浓度，作为演替判定依据
    double totalNutrition = 0.0;
    for (int i = 1; i <= 8; ++i)
    {
        totalNutrition += m_layerEnvList[i].nutrition;
    }
    double avgNut = totalNutrition / 8.0;

    Global::EcoStage newStage;
    if (avgNut > 60.0)
        newStage = Global::EcoStage::Early;
    else if (avgNut > 40.0)
        newStage = Global::EcoStage::Middle;
    else if (avgNut > 20.0)
        newStage = Global::EcoStage::Late;
    else
        newStage = Global::EcoStage::Final;

    // 演替阶段发生变化时输出日志
    if (newStage != m_globalEcoStage)
    {
        m_globalEcoStage = newStage;
        QString stageText;
        switch (newStage)
        {
        case Global::EcoStage::Early: stageText = "生态初期"; break;
        case Global::EcoStage::Middle: stageText = "生态中期"; break;
        case Global::EcoStage::Late: stageText = "生态后期"; break;
        case Global::EcoStage::Final: stageText = "生态末期"; break;
        }
        LOG_INFO(MODULE_NAME, QString("全域生态演替阶段切换为：%1，平均营养值：%2").arg(stageText).arg(avgNut, 0, 'f', 2));
    }
}

void EcologyCore::ClampEnvValue(Global::EnvFactor& factor, int layer)
{
    // 百分比类参数强制限制 0 ~ 100
    auto clamp = [](double& val,const QString& type,int layer)
    {
        if (val < 0.0)
        {
            val = 0.0;
            LOG_WARN("生态数值截断", QString("第%1层环境参数:%2低于下限，已修正为0").arg(layer).arg(type));
        }
        else if (val > 100.0)
        {
            val = 100.0;
            LOG_WARN("生态数值截断", QString("第%1层环境参数:%2高于上限，已修正为100").arg(layer).arg(type));
        }
    };

    clamp(factor.oxygen,"氧气",layer);
    clamp(factor.osmotic,"渗透压",layer);
    clamp(factor.toxin,"毒素浓度",layer);
    clamp(factor.nutrition,"营养物浓度",layer);
}