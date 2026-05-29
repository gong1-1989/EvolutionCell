#ifndef LOGTOOL_H
#define LOGTOOL_H

#include <QObject>
#include <QFile>
#include <QDateTime>
#include <QString>
#include <QMutex>

/**
 * @brief 全局分级日志工具 【静态内核】
 * 功能说明：
 * 1. 支持5级日志：调试/信息/警告/错误/致命错误
 * 2. 双输出：Qt控制台 + 本地日志文件（按日期自动分文件）
 * 3. 线程安全：互斥锁防止多线程日志错乱
 * 4. 性能优化：可全局关闭调试日志，避免高频模块帧率下降
 * 日志路径：项目根目录/Log/yyyy-MM-dd.log
 * 日志内容：全部使用中文描述，便于问题排查
 */
namespace LogTool
{
/**
     * @brief 日志等级枚举（优先级从低到高）
     * LOG_DEBUG  调试日志：开发阶段使用，正式版可关闭
     * LOG_INFO   信息日志：正常流程、模块初始化、状态变更
     * LOG_WARN   警告日志：非致命异常，程序启用兜底逻辑
     * LOG_ERROR  错误日志：局部功能失效，主流程可继续运行
     * LOG_FATAL  致命日志：核心流程中断，存在崩溃风险
     */
enum LogLevel
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
};

/**
     * @brief 初始化日志系统（程序启动第一个调用）
     * @param enableDebug 是否开启调试日志，正式发布建议传 false
     */
void InitLog(bool enableDebug = true);

/**
     * @brief 关闭日志系统（程序退出前调用，释放文件句柄）
     */
void CloseLog();

/**
     * @brief 通用日志写入接口
     * @param level 日志等级
     * @param module 所属模块名称（用于溯源）
     * @param msg 中文日志内容
     */
void WriteLog(LogLevel level, const QString& module, const QString& msg);

// ===================== 快捷调用宏（全局项目通用） =====================
// 调试日志
#define LOG_DBG(module, msg)    LogTool::WriteLog(LogTool::LOG_DEBUG, module, msg)
// 正常信息日志
#define LOG_INFO(module, msg)   LogTool::WriteLog(LogTool::LOG_INFO, module, msg)
// 警告日志
#define LOG_WARN(module, msg)   LogTool::WriteLog(LogTool::LOG_WARN, module, msg)
// 错误日志
#define LOG_ERR(module, msg)    LogTool::WriteLog(LogTool::LOG_ERROR, module, msg)
// 致命错误日志
#define LOG_FATAL(module, msg)  LogTool::WriteLog(LogTool::LOG_FATAL, module, msg)
}

#endif // LOGTOOL_H