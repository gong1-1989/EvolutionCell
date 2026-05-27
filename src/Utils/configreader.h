#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <QJsonObject>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include <QPoint>

/**
 * @brief 全局配置读取工具类
 * @note 合并所有配置到单一 game_config.json，提供通用类型读取接口
 * @feature 自动异常日志、默认值兜底、运行时重载配置
 */
class ConfigReader
{
public:
    // 单例获取
    static ConfigReader& getInstance();
    // 禁止拷贝/移动
    ConfigReader(const ConfigReader&) = delete;
    ConfigReader& operator=(const ConfigReader&) = delete;

    /**
     * @brief 加载全部配置（程序启动调用）
     */
    void loadAllConfig();

    /**
     * @brief 运行时重新加载配置（热更新）
     */
    void reloadAllConfig();

    /**
     * @brief 获取根配置对象
     * @return 全局总配置
     */
    QJsonObject getRootConfig() const;

    // ===================== 通用读取接口（核心：消除重复代码） =====================
    /**
     * @brief 读取int类型配置
     * @param group 父节点字段名
     * @param key 字段名
     * @param defVal 默认兜底值
     * @return 配置值
     */
    int getInt(const QString& group, const QString& key, int defVal = 0) const;

    /**
     * @brief 读取浮点型配置
     * @param group 父节点字段名
     * @param key 字段名
     * @param defVal 默认兜底值
     * @return 配置值
     */
    qreal getDouble(const QString &group, const QString& key, qreal defVal = 0.0) const;
    /**
     * @brief 读取字符型配置
     * @param group 父节点字段名
     * @param key 字段名
     * @param defVal 默认兜底值
     * @return 配置值
     */
    QString getString(const QString& group,const QString& key,QString defVal="")const;
    /**
     * @brief 读取对象型数据
     * @param group 父节点字段名
     * @param key 字段名
     * @param defVal 默认兜底值
     * @return 数据
     */
    QJsonObject getSubObj(const QString& group, const QString& key, const QJsonObject &defVal={})const;

private:
    ConfigReader() = default;
    QJsonObject m_rootCfg;   // 根配置（唯一数据源）
};

#endif // CONFIGREADER_H