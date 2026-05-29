#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QPluginLoader>
#include <QDir>
#include <QMap>
#include "PluginInterface/PluginInterface.h"
#include "Global/GlobalTool.h"
#include "Global/LogTool.h"

/**
 * @brief 全局插件生命周期管理器 【静态内核】
 * 功能：遍历插件目录、加载/卸载DLL、接口校验、帧更新调度
 * 重构升级：全流程异常捕获 + 中文日志，覆盖DLL缺失、接口不匹配、初始化失败等问题
 */
class PluginManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 获取单例实例（全局唯一）
     */
    static PluginManager* GetInstance();

    /**
     * @brief 批量加载目录下所有DLL插件
     * @return true=整体加载成功 false=存在插件加载失败
     */
    bool InitAllPlugins();

    /**
     * @brief 批量卸载所有已加载插件（程序退出调用）
     */
    void UnloadAllPlugins();

    /**
     * @brief 按需加载单个指定插件（DLC/拓展功能使用）
     * @param pluginName 插件名称（不含.dll后缀）
     * @return true=加载成功
     */
    bool LoadSinglePlugin(const QString& pluginName);

    /**
     * @brief 按需卸载单个插件（低配机关闭特效/音频使用）
     * @param pluginName 插件名称
     */
    void UnloadSinglePlugin(const QString& pluginName);

    /**
     * @brief 驱动所有插件执行帧更新（60FPS主循环调用）
     * @param frameCount 全局帧计数器
     */
    void UpdateAllPlugins(int frameCount);

    /**
     * @brief 获取已加载插件列表（调试/事件订阅使用）
     */
    QMap<QString, PluginInterface*> GetLoadedPlugins() const;

private:
    // 私有构造：单例模式，禁止外部实例化
    PluginManager() = default;
    static PluginManager* m_instance;

    QString m_pluginDir;                     // 插件DLL目录路径
    QMap<QString, QPluginLoader*> m_loaderMap;  // 插件加载器映射
    QMap<QString, PluginInterface*> m_pluginMap;// 插件实例映射
};

#endif // PLUGINMANAGER_H