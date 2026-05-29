#ifndef GLOBALTOOL_H
#define GLOBALTOOL_H

#include <QString>
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include "LogTool.h"

/**
 * @brief 全局路径&文件工具类 【静态内核】
 * 功能：统一管理项目所有路径、文件读写、目录创建
 * 重构升级：所有文件/目录操作增加中文异常日志，问题可快速溯源
 * 约束：全项目禁止硬编码路径，所有路径必须通过此类获取
 */
namespace GlobalTool
{
/**
     * @brief 获取项目根目录
     * @return 根目录绝对路径（末尾带/）
     */
QString GetProjectRootPath();

/**
     * @brief 获取JSON配置文件目录 Res/Config/
     */
QString GetConfigPath();

/**
     * @brief 获取文本文案目录 Res/Text/
     */
QString GetTextPath();

/**
     * @brief 获取轮回存档目录 Save/
     */
QString GetSavePath();

/**
     * @brief 获取动态插件DLL目录 Plugins/Bin/
     */
QString GetPluginPath();

/**
     * @brief 读取UTF-8无BOM格式文本文件
     * @param filePath 文件绝对路径
     * @return 文件内容，读取失败返回空字符串，并输出错误日志
     */
QString ReadUtf8File(const QString& filePath);

/**
     * @brief 判断文件是否存在
     * @param filePath 文件绝对路径
     * @return true=存在 false=不存在
     */
bool FileExist(const QString& filePath);

/**
     * @brief 创建目录（支持多级目录）
     * @param dirPath 目录绝对路径
     * @return true=创建/校验成功 false=创建失败，并输出警告日志
     */
bool CreateDir(const QString& dirPath);
}

#endif // GLOBALTOOL_H