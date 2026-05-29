#include "LogTool.h"
#include "GlobalTool.h"
#include <QDebug>

namespace LogTool
{
// 日志文件句柄
static QFile* g_logFile = nullptr;
// 多线程互斥锁：保证日志写入线程安全
static QMutex g_logMutex;
// 调试日志总开关
static bool g_enableDebug = true;
// 模块名（日志工具自身标识）
static const QString g_selfModule = "日志系统";

void InitLog(bool enableDebug)
{
    // 加锁，防止多线程同时初始化
   // QMutexLocker locker(&g_logMutex);
    g_enableDebug = enableDebug;

    // 拼接日志目录路径
    QString logDir = GlobalTool::GetProjectRootPath() + "Log/";
    // 尝试创建日志目录
    if (!GlobalTool::CreateDir(logDir))
    {
        qDebug() << "[警告] 日志目录创建失败，仅输出控制台日志";
        return;
    }

    // 按当前日期命名日志文件（每日一个日志文件）
    QString dateStr = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString logFilePath = logDir + dateStr + ".log";

    // 打开日志文件：追加模式 + 文本模式
    g_logFile = new QFile(logFilePath);
    if (!g_logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        qDebug() << "[错误] 日志文件打开失败，仅输出控制台日志";
        delete g_logFile;
        g_logFile = nullptr;
        return;
    }

    // 写入程序启动标识
    QString bootTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString bootTip = QString("========== 程序启动 时间：%1 ==========\n").arg(bootTime);
    if (g_logFile)
    {
        QTextStream stream(g_logFile);
        stream.setEncoding(QStringConverter::Utf8);
        stream << bootTip;
    }
    qDebug() << "[信息] 日志系统初始化完成，日志文件路径：" << logFilePath;
}

void CloseLog()
{
    QMutexLocker locker(&g_logMutex);
    if (g_logFile != nullptr)
    {
        // 写入程序退出标识
        QString exitTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        QString exitTip = QString("========== 程序退出 时间：%1 ==========\n\n").arg(exitTime);

        QTextStream stream(g_logFile);
        stream.setEncoding(QStringConverter::Utf8);
        stream << exitTip;

        // 关闭并释放文件句柄
        g_logFile->close();
        delete g_logFile;
        g_logFile = nullptr;
    }
    qDebug() << "[信息] 日志系统已正常关闭";
}

void WriteLog(LogLevel level, const QString& module, const QString& msg)
{
    // 关闭调试日志时，直接拦截所有调试信息，提升性能
    if (level == LOG_DEBUG && !g_enableDebug)
    {
        return;
    }

    QMutexLocker locker(&g_logMutex);
    // 拼接时间戳（精确到毫秒）
    QString timeStr = QDateTime::currentDateTime().toString("HH:mm:ss.zzz");
    QString levelText;

    // 日志等级中文映射
    switch (level)
    {
    case LOG_DEBUG:    levelText = "调试";    break;
    case LOG_INFO:     levelText = "信息";    break;
    case LOG_WARN:     levelText = "警告";    break;
    case LOG_ERROR:    levelText = "错误";    break;
    case LOG_FATAL:    levelText = "致命";    break;
    default:           levelText = "未知等级"; break;
    }

    // 组装完整日志内容：[时间] [等级] [模块] 日志信息
    QString fullLog = QString("[%1] [%2] [%3] %4")
                          .arg(timeStr)
                          .arg(levelText, 6)
                          .arg(module)
                          .arg(msg);

    // 1. 输出到Qt控制台
    qDebug() << fullLog;

    // 2. 输出到本地日志文件（文件正常打开时才写入）
    if (g_logFile != nullptr && g_logFile->isOpen())
    {
        QTextStream stream(g_logFile);
        stream.setEncoding(QStringConverter::Utf8);
        stream << fullLog << "\n";
    }
}
}