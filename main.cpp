#include <QApplication>
#include <QFont>
#include "MainWindow/MainWindow.h"
#include "ConfigParser/ConfigParser.h"
#include "PluginManager/PluginManager.h"
#include "EventBus/EventBus.h"
#include "Lineage/LineageManager.h"
#include "Interaction/InteractionManager.h"
#include "Global/LogTool.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 全局默认字体（解决中文显示问题）
    a.setFont(QFont("Microsoft YaHei"));

    const QString MAIN_MODULE = "程序入口";
    LOG_INFO(MAIN_MODULE, "========== 程序启动流程开始 ==========");

    // 【1】第一步：初始化日志系统（所有模块依赖日志，必须最先执行）
    // 正式发布可传入 false 关闭DEBUG日志，提升性能
    LogTool::InitLog(false);
    LOG_INFO(MAIN_MODULE, "步骤1/6：日志系统初始化完成");

    // 【2】第二步：加载全部JSON配置（零硬编码前置依赖）
    ConfigParser::GetInstance()->LoadAllConfig();
    LOG_INFO(MAIN_MODULE, "步骤2/6：全局配置文件加载完成");

    // 【3】第三步：启动事件总线异步消费线程
    EventBus::GetInstance()->StartAsyncConsume();
    LOG_INFO(MAIN_MODULE, "步骤3/6：事件总线异步队列启动完成");

    // 【4】第四步：加载所有动态DLL插件
    PluginManager::GetInstance()->InitAllPlugins();
    LOG_INFO(MAIN_MODULE, "步骤4/6：动态插件加载完成");

    // 【5】第五步：初始化存档、交互子系统
    LineageManager::GetInstance()->InitSaveData();
    InteractionManager::GetInstance()->InitInteraction();
    LOG_INFO(MAIN_MODULE, "步骤5/6：存档系统、交互系统初始化完成");

    // 【6】第六步：启动主窗口与全局主循环
    MainWindow w;
    w.show();
    LOG_INFO(MAIN_MODULE, "步骤6/6：主窗口启动成功，进入60FPS主循环");

    // 进入Qt消息循环
    int exitCode = a.exec();
    LOG_INFO(MAIN_MODULE, QString("主循环退出，程序返回码：%1").arg(exitCode));

    // 退出逆序释放资源
    PluginManager::GetInstance()->UnloadAllPlugins();
    LOG_INFO(MAIN_MODULE, "动态插件全部卸载完毕");

    // 关闭日志系统，释放文件句柄
    LogTool::CloseLog();
    LOG_INFO(MAIN_MODULE, "========== 程序正常退出 ==========\n");

    return exitCode;
}