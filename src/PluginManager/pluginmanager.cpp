#include "PluginManager.h"
#include <QMessageBox>

PluginManager* PluginManager::m_instance = nullptr;
const QString MODULE_NAME = "插件管理器";

PluginManager* PluginManager::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new PluginManager;
        LOG_INFO(MODULE_NAME, "插件管理器单例对象创建成功");
    }
    return m_instance;
}

bool PluginManager::InitAllPlugins()
{
    m_pluginDir = GlobalTool::GetPluginPath();
    QDir pluginDir(m_pluginDir);

    // 校验插件目录是否存在
    if (!pluginDir.exists())
    {
        LOG_WARN(MODULE_NAME, QString("插件目录不存在，自动创建目录：%1").arg(m_pluginDir));
        QDir().mkpath(m_pluginDir);
        return false;
    }

    // 筛选目录下所有DLL文件
    QStringList filter = {"*.dll"};
    QStringList dllFileList = pluginDir.entryList(filter, QDir::Files);
    LOG_INFO(MODULE_NAME, QString("扫描到插件DLL文件总数：%1 个").arg(dllFileList.size()));

    bool totalLoadResult = true;
    int successCount = 0;
    int failCount = 0;

    // 逐个加载DLL插件
    for (const QString& dllName : dllFileList)
    {
        QString fullDllPath = m_pluginDir + dllName;
        QPluginLoader* loader = new QPluginLoader(fullDllPath, this);
        QObject* rawPluginObj = loader->instance();

        // 1. 校验：DLL是否实现标准插件接口
        PluginInterface* plugin = qobject_cast<PluginInterface*>(rawPluginObj);
        if (plugin == nullptr)
        {
            LOG_ERR(MODULE_NAME, QString("DLL接口不匹配，跳过加载：%1，错误详情：%2")
                                     .arg(dllName).arg(loader->errorString()));
            delete loader;
            totalLoadResult = false;
            failCount++;
            continue;
        }

        // 2. 执行插件自身初始化
        QString pluginName = plugin->GetPluginName();
        QString pluginVersion = plugin->GetPluginVersion();
        if (!plugin->InitPlugin())
        {
            LOG_ERR(MODULE_NAME, QString("插件初始化失败：%1 版本：%2").arg(pluginName).arg(pluginVersion));
            delete loader;
            totalLoadResult = false;
            failCount++;
            continue;
        }

        // 3. 加载成功，存入管理容器
        m_loaderMap[pluginName] = loader;
        m_pluginMap[pluginName] = plugin;
        successCount++;
        LOG_INFO(MODULE_NAME, QString("插件加载成功：%1 版本：%2").arg(pluginName).arg(pluginVersion));
    }

    // 输出加载汇总日志
    LOG_INFO(MODULE_NAME, QString("批量加载完成，成功：%1 个，失败：%2 个")
                              .arg(successCount).arg(failCount));
    return totalLoadResult;
}

void PluginManager::UnloadAllPlugins()
{
    LOG_INFO(MODULE_NAME, "开始批量卸载所有插件");
    // 逆序卸载：规避插件之间的依赖关系
    for (auto iter = m_pluginMap.begin(); iter != m_pluginMap.end(); ++iter)
    {
        PluginInterface* plugin = iter.value();
        if (plugin != nullptr)
        {
            plugin->UnloadPlugin();
            LOG_DBG(MODULE_NAME, QString("插件已卸载：%1").arg(plugin->GetPluginName()));
        }
    }

    // 释放加载器资源、清空容器
    qDeleteAll(m_loaderMap.values());
    m_loaderMap.clear();
    m_pluginMap.clear();
    LOG_INFO(MODULE_NAME, "所有插件卸载完成，资源已释放");
}

bool PluginManager::LoadSinglePlugin(const QString& pluginName)
{
    QString fullPath = m_pluginDir + pluginName + ".dll";
    // 检查插件文件是否存在
    if (!GlobalTool::FileExist(fullPath))
    {
        LOG_WARN(MODULE_NAME, QString("按需加载插件失败，文件不存在：%1").arg(pluginName));
        return false;
    }

    QPluginLoader* loader = new QPluginLoader(fullPath, this);
    PluginInterface* plugin = qobject_cast<PluginInterface*>(loader->instance());
    if (plugin == nullptr || !plugin->InitPlugin())
    {
        LOG_ERR(MODULE_NAME, QString("按需加载插件异常：%1，接口不匹配或初始化失败").arg(pluginName));
        delete loader;
        return false;
    }

    m_loaderMap[pluginName] = loader;
    m_pluginMap[pluginName] = plugin;
    LOG_INFO(MODULE_NAME, QString("按需加载插件成功：%1").arg(pluginName));
    return true;
}

void PluginManager::UnloadSinglePlugin(const QString& pluginName)
{
    // 检查插件是否已加载
    if (!m_pluginMap.contains(pluginName))
    {
        LOG_DBG(MODULE_NAME, QString("插件未加载，无需执行卸载：%1").arg(pluginName));
        return;
    }

    PluginInterface* plugin = m_pluginMap[pluginName];
    if (plugin != nullptr)
        plugin->UnloadPlugin();

    // 释放资源
    delete m_loaderMap[pluginName];
    m_loaderMap.remove(pluginName);
    m_pluginMap.remove(pluginName);
    LOG_INFO(MODULE_NAME, QString("按需卸载插件成功：%1").arg(pluginName));
}

void PluginManager::UpdateAllPlugins(int frameCount)
{
    // 同步主循环帧率，驱动所有插件更新
    for (PluginInterface* plugin : m_pluginMap.values())
    {
        if (plugin != nullptr)
            plugin->PluginUpdate(frameCount);
    }
}

QMap<QString, PluginInterface*> PluginManager::GetLoadedPlugins() const
{
    return m_pluginMap;
}