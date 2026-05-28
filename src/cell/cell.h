#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QJsonObject>
#include "physics.h"
#include "eventbus.h"
#include "jsonparser.h"
#include "Utils/randomutil.h"
#include "organellfe.h"

/**
 * @brief class Cell 细胞实体类，核心属性：能量、位置、移动、凋零
 *
 */
class Cell : public QObject
{
    Q_OBJECT
public:
    explicit Cell(QObject *parent=nullptr);
    /**
     * @brief initCell 初始化细胞属性（从配置或者存档文件读取）
     * @param cellCfg QJsonObject数据
     */
    void initCell(const QJsonObject& cellCfg);

    /**
     * @brief updateCell 每一帧更新细胞状态（能量消耗、移动、死亡判定）
     * @param deltaTime 每一帧时间
     */
    void updateCell(qreal deltaTime);

    /**
     * @brief getCenter 获取圆心坐标
     * @return 圆心坐标
     */
    QPointF getCenter()const;

    /**
     * @brief getRadius 获取圆半径
     * @return 圆半径
     */
    qreal getRadius()const;

    /**
     * @brief isAlive 获取细胞是否死亡
     * @return ture=死亡
     */
    bool isAlive()const;

    /**
     * @brief normalizeDir 方向向量归一化（保证移动速度均匀）
     * @param dir 方向向量
     */
    void normalizeDir();

    /**
     * @brief randomChangeDir 随机切换移动方向
     */
    void randomChangeDir();
    /**
     * @brief toJson 序列化接口：用与存档
     * @return QJsonObject数据
     */
    QJsonObject toJson() const;

private:
    /**
     * @brief cellDie 执行细胞凋零逻辑
     */
    void cellDie();
    //======================= 基础属性 ==========================
    qreal m_energy;                             //当前能量
    qreal m_baseEnerygyCost;                    //每秒基础能耗
    qreal m_moveSpeed;                          //移动速度
    int m_cellSize;                             //细胞像素尺寸（默认32px）
    bool m_isAlive;                             //存活状态
    QPointF m_pos;                              //世界坐标
    QPoint m_moveDir;                           //移动方向
    qreal m_walkTime;                           //随机转向定时器

    OrganelleMgr* m_organelleMgr;               //细胞器&分化管理器

    /**
     * @brief The InteractMode enum 交互模式
     */
    enum class InteractMode{
        WatchMode,          //1-纯旁观模式（权限锁全开，禁止引导）
        GuideMode,          //2-趋化引导模式（权重40%）
        DeepMode            //3-深度干预模式（权重60%）
    };

    /**
     * @brief The CellState enum AI状态机
     */
    enum class CellState{
        Normal,             //正常状态
        EngegyWarning,      //能量匮乏
        DangerAvoid,        //危险避险
        SporeSleep          //孢子休眠
    };

    InteractMode m_interactMode;        //当前交互模式（权限锁）
    CellState m_cellState;              //AI状态机
    qreal m_playerWeight;               //玩家总干预权重
};

#endif // CELL_H
