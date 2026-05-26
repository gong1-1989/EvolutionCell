#ifndef PLAYERCELL_H
#define PLAYERCELL_H

#include "Core/GameGlobal.h"
#include "SymbiosisCell.h"
#include "GhostCell.h"
#include "Utils/CollisionUtil.h"
#include <QPointF>
#include <QColor>
#include <QList>
#include <cmath>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

/**
 * @brief 玩家细胞实体
 * 整合：移动、体型成长、躯体解构、基因解锁、共生管理、时空回溯节点、排斥/风险值
 * 核心封装规则：
 * 1. 所有容器/数值为私有，外部禁止直接访问
 * 2. 仅对外提供【行为接口】和【只读查询接口】
 * 3. 内置完整序列化，支持存档读写
 */
class PlayerCell
{
public:
    PlayerCell();

    /**
     * @brief 玩家移动（WASD控制）
     */
    void move(bool keyW, bool keyA, bool keyS, bool keyD, int canvasW, int canvasH);

    /**
     * @brief 体型成长
     * @param addSize 增量大小
     * @param type 吞噬目标类型
     */
    void grow(int addSize, GameGlobal::MonsterType type);

    // ===================== 躯体解构系统 =====================
    /**
     * @brief 执行躯体解构升级
     * @param lv 目标解构等级
     */
    void executeDecompose(GameGlobal::DecomposeLevel lv);

    GameGlobal::DecomposeLevel getDecomposeLevel() const;
    int getDecomposeRisk() const;
    void addDecomposeRisk(int val);

    // ===================== 基因系统 =====================
    /**
     * @brief 解锁新基因
     * @param type 基因类型
     * @return true=解锁成功，false=已拥有
     */
    bool unlockGene(GameGlobal::GeneType type);
    const QList<GameGlobal::GeneType>& getUnlockedGene() const;

    // ===================== 共生体系（严格封装，外部无法直接操作列表） =====================
    /**
     * @brief 添加共生体（自动判断数量上限）
     */
    bool addSymbiosis(const SymbiosisCell& cell);

    /**
     * @brief 清理超时临时共生体
     */
    void clearExpiredSymbiosis(qint64 nowTime);

    /**
     * @brief 清空所有共生体（重置/读档使用）
     */
    void clearAllSymbiosis();

    /**
     * @brief 驱动所有共生体跟随玩家
     */
    void updateSymbiosisFollow(qreal playerX, qreal playerY, const QList<MonsterCell>& monsterList);

    // 共生只读查询
    int getSymbiosisCount() const;
    const QList<SymbiosisCell>& getSymbiosisList() const;

    // 基因排斥值 读写接口
    int getGeneRejectValue() const;
    void setGeneRejectValue(int val);
    void addGeneReject(int val);
    void subGeneReject(int val);
    GameGlobal::RejectLevel getCurrentRejectLevel() const;

    // ===================== 时空回溯 & 历史节点 =====================
    /**
     * @brief 记录当前完整演化快照（关键行为自动调用）
     */
    void recordCurrentEvolveNode();

    /**
     * @brief 回退到上一个历史节点
     * @return true=回溯成功
     */
    bool rollbackToLastNode();

    /**
     * @brief 跳转到指定索引的历史节点
     * @param index 节点下标(0起始)
     * @return true=回溯成功
     */
    bool rollbackToAssignNode(int index);

    int getHistoryNodeCount() const;
    GameGlobal::EvolveHistoryNode getHistoryNodeByIndex(int index) const;

    // ===================== 基础属性查询 =====================
    qreal getX() const;
    qreal getY() const;
    void setPos(qreal x, qreal y);
    int getSize() const;
    GameGlobal::LifeLaw getLawType() const;
    void setLawType(GameGlobal::LifeLaw law);

    // ===================== 序列化接口（全量存档） =====================
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& obj);

    //======================= 拟态伪装系统接口 ==================
    /**
     * @brief 启动拟态伪装
     * @param mimicType 拟态类型（环境/怪物/虚空）
     * @param lastTime 持续时间（毫秒）
     */
    void startMimic(GameGlobal::MimicState mimicType,qint64 lastTime);

    /**
     * @brief 停止拟态跟踪，恢复正常状态
     */
    void stopMimic();

    /**
     * @brief 更新拟态状态（超时自动关闭）
     * @param nowTime 当前时间戳
     */
    void updateMimic(qint64 nowTime);

    //拟态状态查询接口
    GameGlobal::MimicState getMimicState()const;
    bool isInMimc()const;

    //======================== 基因熵变系统接口 ===========================
    /**
     * @brief 主动触发基因熵变（牺牲稳定性换属性）
     * @return true=触发成功
     */
    bool triggerGeneChao();

    /**
     * @brief 修复基因稳定度
     * @param addVal 恢复值
     */
    void repairGeneStable(int addVal);

    //基因状态接口
    int getGeneStableValue()const;
    GameGlobal::GeneStableState getGeneStableState()const;

private:
    // 基础位置、体型
    QPointF m_pos;
    int m_size;

    // 生命法则
    GameGlobal::LifeLaw m_lawType;

    // 躯体解构
    GameGlobal::DecomposeLevel m_decomposeLv;
    int m_currentDecomposeRisk;

    // 基因列表
    QList<GameGlobal::GeneType> m_unlockGene;

    // 共生体系
    QList<SymbiosisCell> m_symbiosisList;
    int m_geneRejectValue;

    // 时空回溯历史节点
    QList<GameGlobal::EvolveHistoryNode> m_historyNodeList;

    //拟态伪装系统
    GameGlobal::MimicState m_mimicState;
    qint64 m_mimicEndTime;
    int m_mimicCoolDown;

    //基因熵变系统
    int m_geneStableValue;
    GameGlobal::GeneStableState m_geneStableState;

};

#endif // PLAYERCELL_H