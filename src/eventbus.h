#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <QObject>
#include <QVariant>
#include <QString>
/**
 * @brief The EventBus class 全局事件总线，模块解耦中心
 */
class EventBus : public QObject
{
    Q_OBJECT
    static EventBus* m_ins;
    explicit EventBus(QObject *parent = nullptr);
public:
    //全局单例
    static EventBus* getInstance();
    //销毁单例
    static void destroyInstance();

    /**
     * @brief dispatchEvent 分发全局事件
     * @param evtName 事件名称（字符串）
     * @param data 事件附加数据（通用变体）
     */
    void dispatchEvent(const QString& evtName,const QVariant&data=QVariant());

signals:
    /**
     * @brief globalEvent 全局事件信号：所有事件统一触发此信号
     * @param evtName 事件名称（字符串）
     * @param data 事件附加数据（通用变体）
     */
    void globalEvent(const QString& evtName,const QVariant&data);

};
//全局事件常量（统一管理，避免魔法字符串）
namespace GlobalEvent {
//=================== 一、【指令事件】上层👉业余模块（玩家引导、控制指令） ========================
//设置细胞分化倾向权重 参数：double 权重：0~1
const QString CMD_SET_DIFF_WEIGHT="CMD_SET_DIFF_WEIGHT";
//设置基因突变倾向权重 参数：double 权重：0~1
const QString CMD_SET_MUTATE_WEIGHT="CMD_SET_MUTATE_WEIGHT";
//=================== 二、【状态事件】业务模块👉上层（数据变更主动推送，代替轮询查询） ======================
//细胞能量变化 参数：double 当前能量
const QString STATE_ENERGY_CHANGE="STATE_ENERGY_CHANGE";
//细胞全能性变化 参数：int 当前全能性0~100
const QString STATE_TOTIPOTENCY_CHANGE="STATE_TOTIPOTENCY_CHANGE";
//恶性突变次数变化 参数：int 累计次数
const QString STATE_MALIGNANT_COUNT="STATE_MALIGNANT_COUNT";
//细胞存活状态变更 参数：bool 是否存活
const QString STATE_ALIVE_STATUS="STATE_ALIVE_STATUS";
//=================== 三、【行为事件】 AI行为执行完成（行为结果通知） ===================
//细胞凋亡 参数：QPointF 死亡坐标
const QString BEHAVE_CELL_DEATH="BEHAVE_CELL_DEATH";
//细胞完成分化 参数 int 分化等级
const QString BEHAVE_CELL_DIFF="BEHAVE_CELLDIFF";
//发生基因突变 参数 int 突变类型
const QString BEHAVE_CELL_MUTATE="BEHAVE_CELL_MUTATE";
//首次水平基因转移（科普弹窗） 无参数
const QString BEHAVE_FIRST_HGT="BEHAVE_FIRST_HGT";
//谱系数据更新  无参数
const QString BEHAVE_LINEAGE_UPDATE="BEHAVE_LINEAGE_UPDATE";
//==================== 四、【系统事件】全局通知（配置、存档、初始化） =====================
//配置文件加载完成 参数：QString配置名称
const QString SYS_CONFIG_LOADED="SYS_CONFIG_LOADED";
//存档完成 参数： int 存档槽位
const QString SYS_SAVE_FINISH="SYS_SAVE_FINISH";
//读档完成 参数： int 存档槽位
const QString SYS_LOAD_FINISH="SYS_LOAD_FINISH";
}

#endif // EVENTBUS_H
