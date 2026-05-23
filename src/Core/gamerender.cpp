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
    p->setBrush(QColor(255,70,70));
    if(!player.hasSpeedBuff()) {
        p->setPen(Qt::NoPen);
    }
    else {
        p->setPen(QPen(QColor(255,100,100),3));
    }
    p->drawEllipse(player.getX()-player.getSize()/2,player.getY()-player.getSize()/2,player.getSize(),player.getSize());
    //==========绘制HUD==========
    int hudX=GameGlobal::HUD_PADDING;
    int hudY=GameGlobal::HUD_PADDING;
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
        p->drawText(canvasRect.center().x()-65,canvasRect.center().y()+20,"按ESC继续游戏");
    }
}
