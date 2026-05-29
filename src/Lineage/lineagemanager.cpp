#include "LineageManager.h"

LineageManager* LineageManager::m_instance = nullptr;

LineageManager* LineageManager::GetInstance()
{
    static QMutex instMutex;
    QMutexLocker locker(&instMutex);

    if (m_instance == nullptr)
    {
        m_instance = new LineageManager;
        LOG_INFO(m_instance->MODULE_NAME, "谱系存档管理器单例创建成功");
    }
    return m_instance;
}

void LineageManager::InitSaveData()
{
    // 拼接存档完整路径
    m_saveFilePath = GlobalTool::GetSavePath() + "lineage_save.json";
    m_currentGeneList.clear();

    // 校验并创建空存档文件
    if (!GlobalTool::FileExist(m_saveFilePath))
    {
        QFile newFile(m_saveFilePath);
        if (newFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            // 写入空JSON对象，避免首次读取格式错误
            newFile.write("{}");
            newFile.close();
            LOG_INFO(MODULE_NAME, QString("存档文件不存在，已创建空存档：%1").arg(m_saveFilePath));
        }
        else
        {
            LOG_ERR(MODULE_NAME, QString("创建存档文件失败，请检查目录权限：%1").arg(m_saveFilePath));
        }
    }
    else
    {
        LOG_INFO(MODULE_NAME, QString("检测到已有存档文件：%1").arg(m_saveFilePath));
    }
}

void LineageManager::SaveCurrentRound(const QVector<Global::Gene>& geneList, int surviveTime)
{
    QJsonArray geneJsonArr;

    // 将基因列表转为JSON数组
    for (const Global::Gene& gene : geneList)
    {
        QJsonObject geneObj;
        geneObj["geneID"] = gene.geneID;
        geneObj["geneName"] = gene.geneName;
        geneObj["geneType"] = gene.geneType;
        geneObj["effectValue"] = gene.effectValue;
        geneObj["keepRound"] = gene.keepRound;
        geneJsonArr.append(geneObj);
    }

    // 组装根对象
    QJsonObject rootObj;
    rootObj["surviveTime"] = surviveTime;
    rootObj["geneList"] = geneJsonArr;

    QJsonDocument doc(rootObj);
    QFile saveFile(m_saveFilePath);

    // 覆盖写入存档
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        LOG_ERR(MODULE_NAME, QString("写入存档失败，文件无法打开：%1").arg(saveFile.errorString()));
        return;
    }

    // 紧凑格式写入JSON
    saveFile.write(doc.toJson(QJsonDocument::Compact));
    saveFile.close();
    LOG_INFO(MODULE_NAME, QString("本轮轮回数据保存完成，生存时长：%1秒，基因数量：%2")
                              .arg(surviveTime).arg(geneList.size()));
}

QVector<Global::Gene> LineageManager::LoadLastRoundGene()
{
    QVector<Global::Gene> resultGeneList;
    // 读取存档文本
    QString fileContent = GlobalTool::ReadUtf8File(m_saveFilePath);
    if (fileContent.isEmpty())
    {
        LOG_WARN(MODULE_NAME, "存档内容为空，无继承基因");
        return resultGeneList;
    }

    // 解析JSON
    QJsonDocument doc = QJsonDocument::fromJson(fileContent.toUtf8());
    if (!doc.isObject())
    {
        LOG_ERR(MODULE_NAME, "存档JSON格式损坏，无法解析，本轮无继承基因");
        return resultGeneList;
    }

    QJsonObject rootObj = doc.object();
    QJsonArray geneArr = rootObj["geneList"].toArray();
    int filterDefect = 0;    // 过滤缺陷基因计数
    int filterExpire = 0;    // 过滤过期隐性基因计数

    // 遍历解析并按规则过滤基因
    for (const QJsonValue& item : geneArr)
    {
        QJsonObject geneObj = item.toObject();
        Global::Gene gene;

        gene.geneID = geneObj["geneID"].toInt(0);
        gene.geneName = geneObj["geneName"].toString("未知基因");
        gene.geneType = geneObj["geneType"].toString("显性");
        gene.effectValue = geneObj["effectValue"].toDouble(0.0);
        gene.keepRound = geneObj["keepRound"].toInt(0);

        // 规则1：缺陷基因 直接过滤，禁止继承
        if (gene.geneType == "缺陷")
        {
            filterDefect++;
            continue;
        }

        // 规则2：隐性基因 轮数-1，轮数≤0则过滤
        if (gene.geneType == "隐性")
        {
            gene.keepRound--;
            if (gene.keepRound <= 0)
            {
                filterExpire++;
                continue;
            }
        }

        // 规则3：显性基因 永久保留，直接加入列表
        resultGeneList.append(gene);
    }

    LOG_INFO(MODULE_NAME, QString("加载上一轮基因完成，原始总数：%1，过滤缺陷：%2，过滤过期隐性：%3，可继承基因：%4")
                              .arg(geneArr.size()).arg(filterDefect).arg(filterExpire).arg(resultGeneList.size()));

    return resultGeneList;
}

void LineageManager::ClearCurrentRound()
{
    m_currentGeneList.clear();
    LOG_DBG(MODULE_NAME, "本轮临时基因数据已清空");
}