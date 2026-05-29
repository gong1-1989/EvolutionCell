#ifndef ECOLOGYCORE_H
#define ECOLOGYCORE_H

#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QRandomGenerator>
#include "Global/GlobalDefine.h"
#include "Global/LogTool.h"
#include "ConfigParser/ConfigParser.h"

/**
 * @brief 立体生态核心计算模块 【静态内核】
 * 架构说明：
 * 1. 底层生态规则、8层纵向环境计算永久静态编译，不拆分DLL
 * 2. 拓展地貌、灾害、特殊生态事件等业务逻辑，拆分至动态插件
 * 对标策划：
 * 1. 第三章 8大纵向生态层级、层间物质传导、界面效应
 * 2. 古菌层周期性波动（V4.0科学规则：200秒周期、8%波动幅度）
 * 3. 全域生态演替四阶段（营养衰减驱动演替）
 * 异常防护：环境数值强制截断0~100，越界输出警告日志
 */
class EcologyCore : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 获取全局单例
     */
    static EcologyCore* GetInstance();

    /**
     * @brief 初始化全域8层生态环境（程序启动调用一次）
     */
    void InitEcology();

    /**
     * @brief 生态全局帧更新（60FPS主循环调用）
     * @param frameCount 全局帧计数器
     */
    void EcologyUpdate(int frameCount);

    /**
     * @brief 获取指定层级的完整环境六因子
     * @param layerId 层级编号 合法范围：1~8
     * @return 环境参数，层级非法返回默认空参数
     */
    Global::EnvFactor GetLayerEnvironment(int layerId) const;

    /**
     * @brief 获取当前全域生态演替阶段
     */
    Global::EcoStage GetGlobalEcoStage() const;

private:
    // 私有构造：单例模式，禁止外部实例化
    EcologyCore() = default;
    static EcologyCore* m_instance;
    const QString MODULE_NAME = "生态核心";

    QVector<Global::EnvFactor> m_layerEnvList;  // 8个纵向层级环境数据（下标0弃用，1~8对应层级）
    Global::EcoStage m_globalEcoStage;           // 全域生态演替阶段
    qint64 m_archaeaFluctTime;                  // 古菌层波动计时戳

    /**
     * @brief 横向地貌对当前层级环境的修正
     * @param layerId 目标层级
     */
    void CalcLandformEffect(int layerId);

    /**
     * @brief 层间物质自上而下传导（上层物质流向下层）
     */
    void TransmitBetweenLayer();

    /**
     * @brief 第8层古菌专属周期性波动（固定200秒周期、±8%幅度）
     */
    void ArchaeaLayerFluctuate();

    /**
     * @brief 气-水界面效应（第1层专属增益）
     */
    void CalcInterfaceBonus();

    /**
     * @brief 根据全域平均营养值，自动切换生态演替阶段
     */
    void UpdateEcoSuccessionStage();

    /**
     * @brief 环境数值边界截断（强制限制 0~100，防止数值溢出）
     * @param factor 待截断的环境参数结构体
     */
    void ClampEnvValue(Global::EnvFactor& factor);
};

#endif // ECOLOGYCORE_H