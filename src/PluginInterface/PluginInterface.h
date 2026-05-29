#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QObject>
#include "Global/GlobalDefine.h"
#include "Global/LogTool.h"

/**
 * @brief 插件统一抽象接口 【静态内核】
 * 架构硬性约束：
 * 1. Qt插件体系要求：该接口必须静态编译进主程序，**禁止拆分为DLL**
 * 2. 所有动态DLL插件必须继承此接口，实现纯虚函数
 * 通信规则：主程序与插件仅通过【事件总线】通信，禁止跨DLL传递对象裸指针
 */

// Qt插件全局唯一标识（固定不可修改）
#define PLUGIN_INTERFACE_IID "com.CellEra.PluginInterface.v5.0"

class PluginInterface
{
public:
    // 虚析构：保证插件对象安全释放
    virtual ~PluginInterface() = default;

    /**
     * @brief 插件初始化函数（插件加载时执行一次）
     * @return true=初始化成功 false=初始化失败（管理器会卸载该插件）
     */
    virtual bool InitPlugin() = 0;

    /**
     * @brief 插件卸载函数（程序退出/按需关闭插件时执行）
     */
    virtual void UnloadPlugin() = 0;

    /**
     * @brief 插件帧更新函数（同步主程序60FPS全局循环）
     * @param frameCount 全局帧计数器
     */
    virtual void PluginUpdate(int frameCount) = 0;

    /**
     * @brief 获取插件名称（用于日志、管理、版本校验）
     */
    virtual QString GetPluginName() const = 0;

    /**
     * @brief 获取插件版本号（兼容性校验，拒绝加载旧版本不兼容插件）
     */
    virtual QString GetPluginVersion() const = 0;
};

// Qt插件标准宏：声明接口
Q_DECLARE_INTERFACE(PluginInterface, PLUGIN_INTERFACE_IID)

#endif // PLUGININTERFACE_H