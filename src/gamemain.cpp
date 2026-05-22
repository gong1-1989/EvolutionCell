#include "gamemain.h"

GameMain::GameMain(QWidget *parent)
    : QWidget{parent}
    , m_gameLoop(new GameLoop)
    , m_player(new Cellentity)
    ,m_levelSys(new LevelSystem)
{
    this->setFixedSize(1280, 720);
    this->setWindowTitle("进化亿重奏:细胞纪元");
    this->setFocusPolicy(Qt::StrongFocus);
    //初始化游戏循环
    connect(m_gameLoop, &GameLoop::frameUpdate, this, &GameMain::UpdateGame);
    m_gameLoop->startLoop();
    m_spawnTimer = new QTimer(this);
    m_spawnTimer->setInterval(2000);
    connect(m_spawnTimer, &QTimer::timeout, this, &GameMain::spawnMonster);
    m_spawnTimer->start();
    m_muscleCells.append(MuscleCell());
    m_shieldCells.append(ShieldCell());
    m_magicCells.append(MagicCell());
}

void GameMain::movePlayer(float detaTime){
    //计算移动偏量
    float moveX = 0, moveY = 0;
    if (m_keyW)
        moveY -= 1;
    if (m_keyS)
        moveY += 1;
    if (m_keyA)
        moveX -= 1;
    if (m_keyD)
        moveX += 1;
    //归一化
    if (moveX != 0 && moveY != 0) {
        float len = qSqrt(moveX * moveX + moveY * moveY);
        moveX /= len;
        moveY /= len;
    }
    float dx = moveX * m_player->m_moveSpeed * detaTime;
    float dy = moveY * m_player->m_moveSpeed * detaTime;
    m_player->moveCell(dx, dy);
    m_player->limitBoundary(this->rect());
}
void GameMain::UpdateGame(float deltaTime)
{
    movePlayer(deltaTime);
    for(MuscleCell &cell:m_muscleCells){
        cell.cellAttack(m_monsters,deltaTime);
        cell.moveChase(cell.m_pos,m_monsters,deltaTime);
    }
    for(ShieldCell &cell:m_shieldCells){
        cell.cellAttack(m_monsters,deltaTime);
        cell.moveChase(cell.m_pos,m_monsters,deltaTime);
    }
    for(MagicCell &cell:m_magicCells){
        cell.cellAttack(m_monsters,deltaTime);
        cell.moveChase(cell.m_pos,m_monsters,deltaTime);
    }

    QList<MonsterEntity> aliveMon;
    int gainExp=0;
    for (MonsterEntity &mon : m_monsters) {
        qreal dirX = m_player->m_pos.x() - mon.m_pos.x();
        qreal dirY = m_player->m_pos.y() - mon.m_pos.y();
        qreal len = qSqrt(dirX * dirX + dirY * dirY);
        if (len > 0) {
            dirX /= len;
            dirY /= len;
            mon.moveCell(dirX * mon.m_moveSpeed * deltaTime, dirY * mon.m_moveSpeed * deltaTime);
        }
        mon.limitBoundary(this->rect());
        if(mon.m_hp>0) aliveMon.append(mon);
        else gainExp+=mon.m_expDrop;
    }
    m_monsters.swap(aliveMon);

    if(gainExp>0){
        m_playerExp+=gainExp;
        while(m_playerExp>=m_expNeed){
            m_playerExp-=m_expNeed;
            m_playerLevel++;
            m_expNeed=qRound(m_expNeed*1.2);
            m_player->m_hp+=15;
            m_player->m_attack+=2;
        }
    }
    this->update(); //触发重绘
}
void GameMain::cellAttackMonster(Cellentity &cell, float deltaTime){
    cell.m_cdTimer+=deltaTime;
    if(cell.m_cdTimer<cell.m_attackCd)return;
    for(MonsterEntity& mon:m_monsters){
        qreal disX=cell.m_pos.x()-mon.m_pos.x();
        qreal disY=cell.m_pos.y()-mon.m_pos.y();
        qreal distance=qSqrt(disX*disX+disY*disY);
        if(distance<=cell.m_attackRange){
            mon.m_hp-=cell.m_attack;
            cell.m_cdTimer=0;
            break;
        }
    }
}

void GameMain::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter p(this);
    p.fillRect(this->rect(), QColor(15, 15, 25));
    p.setBrush(m_player->m_cellcolor);
    p.setPen(Qt::NoPen);
    p.drawEllipse(m_player->m_pos, m_player->m_radius, m_player->m_radius);
    //p.drawPixmap(player->rect,player->pix);
    for (MuscleCell &cell : m_muscleCells) {
        p.setBrush(cell.m_cellcolor);
        p.drawEllipse(cell.m_pos, cell.m_radius, cell.m_radius);
    }
    for (ShieldCell &cell : m_shieldCells) {
        p.setBrush(cell.m_cellcolor);
        p.drawEllipse(cell.m_pos, cell.m_radius, cell.m_radius);
    }
    for (MagicCell &cell : m_magicCells) {
        p.setBrush(cell.m_cellcolor);
        p.drawEllipse(cell.m_pos, cell.m_radius, cell.m_radius);
    }
    for (MonsterEntity &cell : m_monsters) {
        p.setBrush(cell.m_cellcolor);
        p.drawEllipse(cell.m_pos, cell.m_radius, cell.m_radius);
    }
    p.setPen(Qt::white);
    p.drawText(20,30,QString("等级：%1 经验：%2/%3")
                           .arg(m_levelSys->getLevel())
                           .arg(m_levelSys->getCurExp())
                           .arg(m_levelSys->getNeedExp()));
}
void GameMain::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        m_keyW = true;
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        m_keyS = true;
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        m_keyA = true;
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        m_keyD = true;
        break;
    default:
        break;
    }
}
void GameMain::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        m_keyW = false;
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        m_keyS = false;
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        m_keyA = false;
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        m_keyD = false;
        break;
    default:
        break;
    }
}
void GameMain::spawnMonster()
{
    MonsterEntity mon;
    qreal x = QRandomGenerator::global()->bounded(50, width() - 50);
    qreal y = QRandomGenerator::global()->bounded(50, height() - 50);
    mon.m_pos = QPointF(x, y);
    m_monsters.append(mon);
}