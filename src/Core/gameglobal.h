#ifndef GAMEGLOBAL_H
#define GAMEGLOBAL_H

#include <QColor>
//#include "Utils/ConfigReader.h"
#include <QList>

/**
 * @brief 全局定义模块
 * 1. 所有枚举、结构体、全局常量
 * 2. 配置读取接口（仅做转发，无重复解析代码）
 * 3. 严格分组、详细注释，对标策划规则
 */
namespace GameGlobal
{
// ===================== 一、基础状态枚举 =====================
/**
 * @brief 游戏运行状态
 */
enum class GameState
{
    RUNNING,    // 正常运行
    PAUSED      // 游戏暂停
};

/**
 * @brief 怪物类型（策划5.2）
 */
enum class MonsterType
{
    NORMAL,     // 普通怪物
    ELITE,      // 精英怪物
    SPECIAL     // 特殊怪物
};

/**
 * @brief 基因属性（策划8.1）（进化解锁，不达门槛不可用）
 */
enum class GeneAttr
{
    None,Metal,Wood, Water, Fire,Earth
};

/**
 * @brief 能力类型（进化解锁，不达门槛不可用）
 */
enum class AbilityType{
    Split,                  //分裂（基础）
    Symbiosis,              //共生
    GhostRollback,          //时空残影
    Mimic,                  //拟态
    GeneChao                //基因熵变
};

/**
 * @brief 吞噬结果类型
 */
enum class EatResult{
    Win,                    //吞噬成功
    Lose,                   //被吞噬
    Draw                    //势均力敌，互伤
};

/**
 * @brief 变异等级
 */
enum class MutationLevel{Normal,Rare,Epic,Legend};

/**
 * @brief 三大生命法则（策划第四章）
 */
enum class LifeLaw
{
    LAW_FISSION,    // 裂变法则
    LAW_SYMBIOSIS,  // 共生法则
    LAW_ILLUSION    // 虚妄法则
};

/**
 * @brief 躯体解构等级（策划5.1 四级不可逆）
 */
enum class DecomposeLevel
{
    DECOMPOSE_NONE,     // 原生形态
    DECOMPOSE_LIGHT,    // 轻度解构
    DECOMPOSE_DEEP,     // 深度解构
    DECOMPOSE_FULL      // 完全重构
};

/**
 * @brief 演化素材类型
 */
enum class EvolutionMaterial
{
    MATERIAL_ORGAN,
    MATERIAL_GENE,
    MATERIAL_LAW_CRYST
};

// ===================== 二、共生体系枚举（策划5.2） =====================
/**
 * @brief 共生模式
 */
enum class SymbiosisMode
{
    SYMBIO_TEMP,     // 临时寄生：限时存在，超时销毁
    SYMBIO_PERM,     // 永久共生：常驻族群
    SYMBIO_ABSORB    // 吞噬同化：完全转为自身族群
};

/**
 * @brief 基因排斥等级
 */
enum class RejectLevel
{
    REJECT_SAFE,     // 安全无惩罚
    REJECT_WARNING,  // 轻度排斥：属性衰减
    REJECT_DANGER    // 高危紊乱：大幅属性衰减
};

// ===================== 三、时空回溯枚举（策划5.3） =====================
enum class RollbackOperate
{
    ROLLBACK_LAST,      // 回退上一节点
    ROLLBACK_ASSIGN     // 指定节点回溯
};

// ===================== 四、演化历史节点结构体（存档/回溯专用） =====================
/**
 * @brief 演化快照节点
 * 记录关键演化行为的完整状态，用于时空回溯
 */
struct EvolveHistoryNode
{
    qreal posX;                 // 位置X
    qreal posY;                 // 位置Y
    int bodySize;               // 体型大小
    LifeLaw currentLaw;         // 当前生命法则
    DecomposeLevel decomposeLv; // 解构等级
    int decomposeRisk;          // 畸变风险值
    int geneRejectValue;        // 基因排斥值
    QList<int> unlockGeneType;  // 已解锁基因列表
};

// ===================== 五、全局固定常量（清理所有硬编码） =====================
namespace Const
{
const qreal CELL_MOVE_STEP      = 1.2;    // 细胞基础移动步长
const qreal COLLISION_OFFSET    = 5.0;    // 碰撞判定补偿像素
const int HUD_TEXT_OFFSET_Y     = 20;     // HUD文字纵向偏移
const qreal GHOST_MOVE_SPEED    = 2.0;    // 残影移动速度
const qreal SYM_MOVE_SPEED      = 2.5;    // 共生体移动速度
}

// ===================== 六、拟态伪装枚举 =================================
/**
 * @brief 拟态状态
 * 形态拟态伪装系统核心，控制怪物仇恨、偷袭暴击、暴露概率
 */
enum MimicState
{
    MIMIC_NONE,             //无拟态：正常状态，怪物正常追击
    MIMIC_ENV,              //环境拟态：降低仇恨，避免环境伤害
    MIMIC_MONSTER,          //怪物拟态：混入怪物，近身触发偷袭暴击
    MIMIC_VOID              //虚空拟态：短暂无敌，规避所有伤害
};

// ===================== 七、基因熵变枚举 =================================
/**
 * @brief 基因稳定状态
 * 基因熵变系统核心，控制畸变概率、属性增益/衰减
 */
enum GeneStableState
{
    STABLE_SAFE,                //安全状态：无畸变，属性正常
    STABLE_WARN,                //轻微紊乱：低概率畸变，小幅度属性波动
    STABLE_DANGER               //高危熵变：高概率畸变，大幅度属性暴涨、衰减
};

// ===================== 八、配置读取接口（纯转发，无重复解析） =====================

// 躯体解构配置
int getDecomposeRisk(DecomposeLevel lv);
} // namespace GameGlobal

#endif // GAMEGLOBAL_H