#include "gamerender.h"

void GameRender::drawAll(QPainter *p, const GameManager &gameMgr, const QRect &canvasRect){
    p->setRenderHint(QPainter::Antialiasing);
    p->fillRect(canvasRect,QColor(18,22,35));//画布背景深色
    auto& monList=gameMgr.getMonsterList();
    for(const MonsterCell &m:monList){
        p->setBrush(m.getColor());
        if(m.getType()==GameGlobal::NORAMAL){
            p->setPen(Qt::NoPen);
        }else if(m.getType()==GameGlobal::ELITE){
            p->setPen(QPen(QColor(255,215,0),2));

        }else if(m.getType()==GameGlobal::SPECIAL){
            p->setPen(QPen(QColor(138,43,226),1));
        }
        p->drawEllipse(m.getX()-m.getSize()/2,m.getY()-m.getSize()/2,m.getSize(),m.getSize());
    }
    auto& player=gameMgr.getPlayer();
    GameGlobal::LifeLaw law=gameMgr.getPlayerLawType();
    QColor playerColor;
    switch (law) {
    case GameGlobal::LAW_FISSION:
        playerColor=QColor(255,42,42);
        break;
    case GameGlobal::LAW_SYMBIOSIS:
        playerColor=QColor(42,209,104);
        break;
    case GameGlobal::LAW_ILLUSION:
        playerColor=QColor(123,42,209);
        break;
    default:
        playerColor=QColor(255,70,70);
        break;
    }
    p->setBrush(playerColor);
    p->setPen(Qt::NoPen);
    p->drawEllipse(player.getX()-player.getSize()/2,player.getY()-player.getSize()/2,player.getSize(),player.getSize());
    if(!player.hasSpeedBuff()) {
        p->setPen(Qt::NoPen);
    }
    else {
        p->setPen(QPen(QColor(255,100,100),3));
    }
    p->drawEllipse(player.getX()-player.getSize()/2-2
                   ,player.getY()-player.getSize()/2-2
                   ,player.getSize()+4,player.getSize()+4);

    auto&symList=gameMgr.getSymbiosisList();
    for(const auto& symCell:symList){
        p->setBrush(symCell.getColor());
        p->setPen(Qt::white);
        int sz=symCell.getSize();
        p->drawEllipse(symCell.getX()-sz/2,symCell.getY()-sz/2,sz,sz);
    }
    //==========绘制HUD==========
    int hudX=GameGlobal::HUD_PADDING;
    int hudY=GameGlobal::HUD_PADDING;
    p->setPen(GameGlobal::HUD_TEXT_COLOR);
    QString lawText;
    switch (law) {
    case GameGlobal::LAW_FISSION:
        lawText="当前法则：裂变解构";
        break;
    case GameGlobal::LAW_SYMBIOSIS:
        lawText="当前法则：菌群共生";
        break;
    case GameGlobal::LAW_ILLUSION:
        lawText="当前法则：时空虚妄";
        break;
    default:
        break;
    }
    p->drawText(hudX+80,hudY+30,lawText);

    QString decText;
    auto decompLv=gameMgr.getplayerDecomposeLv();
    switch (decompLv) {
    case GameGlobal::DECOMPOSE_NONE:
        decText="躯体状态：原生形态";
        break;
    case GameGlobal::DECOMPOSE_LIGHT:
        decText="躯体状态：微拆解蜕变";
        break;
    case GameGlobal::DECOMPOSE_DEEP:
        decText="躯体状态：深度解构";
        break;
    case GameGlobal::DECOMPOSE_FULL:
        decText="躯体状态：完全重构";
        break;
    default:
        break;
    }    
    p->drawText(hudX+80,hudY+50,decText);

    int symNum=gameMgr.getPlayerSymbiosisNum();
    QString symText=QString("共生菌体数量：%1/%2").arg(symNum).arg(GameGlobal::getMaxSymbiosisCount());
    p->drawText(hudX+180,hudY+35,symText);
    QString rejectText;
    QColor rejectColeor;
    auto rejectLv=gameMgr.getPlayerRejectLevel();
    switch (rejectLv) {
    case GameGlobal::REJECT_SAFE:
        rejectText="基因状态：稳定安全";
        rejectColeor=QColor(80,220,80);
        break;
    case GameGlobal::REJECT_WARNING:
        rejectText="基因状态：轻度排斥";
        rejectColeor=QColor(220,180,60);
        break;
    case GameGlobal::REJECT_DANGER:
        rejectText="基因状态：高危紊乱";
        rejectColeor=QColor(220,60,60);
        break;
    }
    p->setPen(rejectColeor);
    p->drawText(hudX+180,hudY,rejectText);

    QString riskText=QString("躯体畸变风险值：%1").arg(gameMgr.getPlayerDecomposeRisk());
    p->setPen(QColor(255,120,120));
    p->drawText(hudX+80,hudY+70,riskText);
    p->setPen(Qt::NoPen);
    p->setBrush(GameGlobal::PROGRESS_BG_COLOR);
    p->drawRect(hudX,hudY,GameGlobal::HUD_BAR_WIDTH,GameGlobal::HUD_BAR_HEIFHT);
    qreal progress=(qreal)player.getSize()/GameGlobal::getPlayerMaxSize();
    p->setBrush(GameGlobal::PROGRESS_FG_COLOR);
    p->drawRect(hudX,hudY,GameGlobal::HUD_BAR_WIDTH*progress,GameGlobal::HUD_BAR_HEIFHT);
    p->setPen(GameGlobal::HUD_TEXT_COLOR);
    p->drawText(hudX,hudY-5,QString("体型：%1/%2").arg(player.getSize()).arg(GameGlobal::getPlayerMaxSize()));
    int eatX=canvasRect.width()-GameGlobal::HUD_PADDING-80;
    int eatY=GameGlobal::HUD_PADDING+20;
    p->drawText(eatX,eatY,QString("吞噬：%1").arg(gameMgr.getEacCount()));
    if(player.hasSpeedBuff()){
        p->setPen(QColor(255,100,100));
        p->drawText(hudX,hudY+GameGlobal::HUD_BAR_HEIFHT+15,"⚡速度提升中！");
    }
    //=====绘制已解锁基因
    int geneY=hudY+GameGlobal::HUD_BAR_HEIFHT+40;
    p->setPen(QColor(100,200,255));
    p->drawText(hudX,geneY,"已解锁基因：");
    const auto& geneList=gameMgr.getPlayer().getUnlockedGene();
    int offsetY=20;
    for(const auto& gene:geneList){
        p->setPen(GameGlobal::HUD_TEXT_COLOR);
        p->drawText(hudX+10,geneY+offsetY,gene.getName());
        offsetY+=18;
    }
    //=====绘制暂停界面
    if(gameMgr.getGameState()==GameGlobal::PAUSED){
        p->setBrush(QColor(0,0,0,180));
        p->drawRect(canvasRect);
        p->setPen(QColor(255,255,255));
        p->setFont(QFont("Arial",24,QFont::Bold));
        p->drawText(canvasRect.center().x()-60,canvasRect.center().y()-20,"游戏暂停");
        p->setFont(QFont("Arial",12));
        p->drawText(canvasRect.center().x()-65,canvasRect.center().y()+20,"按ESC继续游戏|按T键快捷存档");
    }
}
