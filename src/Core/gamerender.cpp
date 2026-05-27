#include "GameRender.h"

void GameRender::drawCircleCell(QPainter* painter, qreal x, qreal y, int size, const QColor& color, bool transparent)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing); // 抗锯齿

    QBrush brush(color);
    QPen pen(Qt::transparent); // 默认无边框

    // 残影/半透明实体关闭描边，普通实体白色细边框
    if (!transparent)
        pen = QPen(Qt::white, 1);

    painter->setBrush(brush);
    painter->setPen(pen);
    painter->drawEllipse(x - size/2.0, y - size/2.0, size, size);
    painter->restore();
}

void GameRender::drawBackground(QPainter* painter, int canvasW, int canvasH)
{
    painter->fillRect(0, 0, canvasW, canvasH, QColor(20, 25, 35));
}

void GameRender::drawMonster(QPainter* painter)
{
    GameManager& mgr = GameManager::getInstance();
    // 【合规】调用公有只读接口，不访问私有成员
    const QList<MonsterCell>& monsterList = mgr.getMonsterList();

    for (const auto& mon : monsterList)
    {
        drawCircleCell(painter, mon.getX(), mon.getY(), mon.getSize(), mon.getColor());
    }
}

void GameRender::drawSymbiosis(QPainter* painter)
{
    GameManager& mgr = GameManager::getInstance();
    const QList<SymbiosisCell>& symList = mgr.getSymbiosisList();
    for (const auto& sym : symList)
    {
        drawCircleCell(painter, sym.getX(), sym.getY(), sym.getSize(), sym.getColor());
    }
}

void GameRender::drawGhost(QPainter* painter)
{
    GameManager& mgr = GameManager::getInstance();
    const QList<GhostCell>& ghostList = mgr.getGhostList();
    for (const auto& ghost : ghostList)
    {
        // 残影标记为半透明绘制
        drawCircleCell(painter, ghost.getX(), ghost.getY(), ghost.getSize(), ghost.getColor(), true);
    }
}

void GameRender::drawPlayer(QPainter* painter)
{
    GameManager& mgr = GameManager::getInstance();
    // 【合规】调用公有只读接口
    const PlayerCell& player = mgr.getPlayer();

    // 玩家主体配色（按生命法则区分）
    QColor playerColor;
    switch (player.getLawType())
    {
    case GameGlobal::LAW_FISSION:    playerColor = QColor(255, 60, 60); break;
    case GameGlobal::LAW_SYMBIOSIS:  playerColor = QColor(60, 255, 100); break;
    case GameGlobal::LAW_ILLUSION:   playerColor = QColor(120, 60, 255); break;
    default: playerColor = QColor(255,255,255); break;
    }

    drawCircleCell(painter, player.getX(), player.getY(), player.getSize(), playerColor);
}

void GameRender::drawHUD(QPainter* painter)
{
    GameManager& mgr = GameManager::getInstance();
    painter->save();
    painter->setPen(Qt::white);
    painter->setFont(QFont("Microsoft YaHei", 10));

    int offsetY = GameGlobal::Const::HUD_TEXT_OFFSET_Y;
    int baseX = 10;

    // 1. 基础状态
    painter->drawText(baseX, offsetY, QString("吞噬总数：%1").arg(mgr.getEatTotal()));
    offsetY += 20;

    // 2. 共生体数量
    int symNum = mgr.getPlayerSymbiosisNum();
    int maxSym = ConfigReader::getInstance().getInt("monster_setting", "max_count", 18);
    painter->drawText(baseX, offsetY, QString("共生体：%1 / %2").arg(symNum).arg(maxSym));
    offsetY += 20;

    // 3. 基因排斥状态
    QString rejectText;
    switch (mgr.getPlayerRejectLevel())
    {
    case GameGlobal::REJECT_SAFE:     rejectText = "基因状态：正常"; break;
    case GameGlobal::REJECT_WARNING:  rejectText = "基因状态：轻度排斥"; break;
    case GameGlobal::REJECT_DANGER:   rejectText = "基因状态：高危紊乱"; break;
    }
    painter->drawText(baseX, offsetY, rejectText);
    offsetY += 20;

    // 4. 时空节点 & 残影
    int nodeCnt = mgr.getHistoryNodeTotal();
    int maxNode = ConfigReader::getInstance().getInt("rollback_ghost_setting", "max_history_node", 8);
    painter->drawText(baseX, offsetY, QString("演化节点：%1 / %2").arg(nodeCnt).arg(maxNode));
    offsetY += 20;

    int ghostCnt = mgr.getGhostCount();
    painter->drawText(baseX, offsetY, QString("时空残影：%1 个").arg(ghostCnt));
    offsetY += 20;

    //5.拟态状态显示
    QString mimicText;
    switch (mgr.getPlayerMimicStable()) {
    case GameGlobal::MIMIC_NONE: mimicText="拟态：无";break;
    case GameGlobal::MIMIC_ENV: mimicText="拟态：环境伪装";break;
    case GameGlobal::MIMIC_MONSTER: mimicText="拟态：怪物伪装";break;
    case GameGlobal::MIMIC_VOID: mimicText="拟态：虚空无敌";break;
    }
    painter->drawText(baseX,offsetY,mimicText);
    offsetY+=20;

    //6.基因稳定度显示
    QString geneStableText=QString("基因稳定度：%1%").arg(mgr.getPlayerGeneStable());
    painter->drawText(baseX,offsetY,geneStableText);

    painter->restore();
}

void GameRender::renderAll(QPainter* painter, int canvasW, int canvasH)
{
    // 严格按层级顺序绘制
    drawBackground(painter, canvasW, canvasH);
    drawMonster(painter);
    drawSymbiosis(painter);
    drawGhost(painter);
    drawPlayer(painter);
    drawHUD(painter);
}