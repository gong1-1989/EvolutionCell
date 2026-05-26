#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QDebug>

/**
 * @brief 全局存档管理器（单例）
 * 特性：
 * 1. 时间戳命名存档，无限存档槽位
 * 2. 存档根节点携带版本号，兼容新旧版本
 * 3. 自动创建存档目录、文件读写异常捕获
 * 4. 基于实体内置 toJson/fromJson 做统一序列化，消除冗余代码
 */
class SaveManager
{
public:
    // 单例模式
    static SaveManager& getInstance();
    SaveManager(const SaveManager&) = delete;
    SaveManager& operator=(const SaveManager&) = delete;

    /**
     * @brief 创建新存档（时间戳命名）
     * @param rootData 游戏全量数据JSON
     * @return 存档文件路径，空=失败
     */
    QString createNewSave(const QJsonObject& rootData);

    /**
     * @brief 加载指定存档
     * @param filePath 存档文件路径
     * @return 解析后的JSON对象，空对象=加载失败/文件损坏
     */
    QJsonObject loadSave(const QString& filePath);

    /**
     * @brief 获取所有存档文件路径列表
     */
    QList<QString> getSaveFileList() const;

    /**
     * @brief 获取存档根目录
     */
    QString getSaveDir() const;

    /**
     * @brief 获取存档文件的完整路径
     */
    QString getSaveFullPath(const QString& fileName)const;

    /**
     * @brief 删除指定存档文件
     */
    bool deleteSave(const QString& fullPath);

private:
    SaveManager() = default;

    /**
     * @brief 确保存档目录存在，不存在则创建
     */
    bool checkAndCreateDir() const;

    // 存档版本号（用于版本兼容）
    const QString m_saveVersion = "1.0";
};

#endif // SAVEMANAGER_H