#include "GlobalTool.h"

namespace GlobalTool
{
const QString MODULE_NAME = "全局文件工具";

QString GetProjectRootPath()
{
    // 程序运行目录向上跳转一级 = 项目根目录
    QString exePath = QCoreApplication::applicationDirPath();
    QDir dir(exePath);
    dir.cdUp();
    return dir.absolutePath() + "/";
}

QString GetConfigPath()
{
    return GetProjectRootPath() + "Res/Config/";
}

QString GetTextPath()
{
    return GetProjectRootPath() + "Res/Text/";
}

QString GetSavePath()
{
    QString saveDir = GetProjectRootPath() + "Save/";
    CreateDir(saveDir);
    return saveDir;
}

QString GetPluginPath()
{
    return GetProjectRootPath() + "Plugins/Bin/";
}

QString ReadUtf8File(const QString& filePath)
{
    // 第一步：判断文件是否存在
    if (!FileExist(filePath))
    {
        LOG_WARN(MODULE_NAME, QString("目标文件不存在：%1").arg(filePath));
        return "";
    }

    QFile file(filePath);
    // 第二步：尝试打开文件
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        LOG_ERR(MODULE_NAME, QString("文件打开失败：%1，错误信息：%2")
                                 .arg(filePath).arg(file.errorString()));
        return "";
    }

    // 第三步：UTF-8编码读取内容
    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    QString content = stream.readAll();
    file.close();

    LOG_DBG(MODULE_NAME, QString("文件读取成功：%1，文本长度：%2")
                             .arg(filePath).arg(content.length()));
    return content;
}

bool FileExist(const QString& filePath)
{
    return QFile::exists(filePath);
}

bool CreateDir(const QString& dirPath)
{
    QDir dir;
    bool result = dir.mkpath(dirPath);
    if (result)
    {
        LOG_DBG(MODULE_NAME, QString("目录校验/创建成功：%1").arg(dirPath));
    }
    else
    {
        LOG_WARN(MODULE_NAME, QString("目录创建失败：%1，请检查目录权限").arg(dirPath));
    }
    return result;
}
}