#ifndef LINEAGEMANAGER_H
#define LINEAGEMANAGER_H

#include <QObject>
#include <QVector>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "Global/GlobalDefine.h"
#include "Global/GlobalTool.h"
#include "Global/LogTool.h"

/**
 * @brief 基因谱系&轮回存档管理器 【静态内核】
 * 架构说明：
 * 1. 核心基因继承、存档读写逻辑永久静态，拓展基因编辑功能拆分至插件
 * 2. 存档规则（策划3.7）：
 *    - 显性基因：永久继承
 *    - 隐性基因：最多保留3个轮回，轮数耗尽自动清除
 *    - 缺陷基因：禁止继承，直接过滤
 * 异常防护：文件不存在、JSON损坏、格式错误、解析失败 全场景日志+兜底
 */
class LineageManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 获取全局单例
     */
    static LineageManager* GetInstance();

    /**
     * @brief 初始化存档系统（创建存档目录、校验存档文件）
     */
    void InitSaveData();

    /**
     * @brief 保存当前轮回数据（基因列表 + 本局生存时长）
     * @param geneList 本局细胞基因集合
     * @param surviveTime 本局总生存时长(秒)
     */
    void SaveCurrentRound(const QVector<Global::Gene>& geneList, int surviveTime);

    /**
     * @brief 加载上一轮可继承基因（按规则过滤）
     * @return 过滤后的合法基因列表
     */
    QVector<Global::Gene> LoadLastRoundGene();

    /**
     * @brief 清空本轮临时数据（新轮回启动时调用）
     */
    void ClearCurrentRound();

private:
    LineageManager() = default;
    static LineageManager* m_instance;
    const QString MODULE_NAME = "谱系存档管理器";

    QString m_saveFilePath;                // 存档文件绝对路径
    QVector<Global::Gene> m_currentGeneList;// 本轮临时基因缓存
};

#endif // LINEAGEMANAGER_H