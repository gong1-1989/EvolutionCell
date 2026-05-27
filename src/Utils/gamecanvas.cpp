#include "GameCanvas.h"

GameCanvas::GameCanvas(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(this->parentWidget()->width(),this->parentWidget()->height());
    setFocusPolicy(Qt::StrongFocus); // 捕获键盘焦点

    // 1. 帧定时器 60FPS ≈ 16ms/帧
    m_frameTimer = new QTimer(this);
    m_frameTimer->setInterval(1000/ConfigReader::getInstance().getInt("game_setting","fps",60));
    connect(m_frameTimer, &QTimer::timeout, this, &GameCanvas::onFrameUpdate);
    m_frameTimer->start();

    // 2. 按键防抖定时器 200ms
    m_keyDebounceTimer = new QTimer(this);
    m_keyDebounceTimer->setInterval(200);
    m_keyDebounceTimer->setSingleShot(true);
    connect(m_keyDebounceTimer, &QTimer::timeout, this, &GameCanvas::resetKeyState);

    // 初始化游戏场景
    GameManager::getInstance().initScene(width(), height());
    qDebug() << "游戏画布初始化完成";
}

void GameCanvas::onFrameUpdate()
{
    //暂停是不更新逻辑
    if(GameManager::getInstance().getGameState()==GameGlobal::PAUSED){
        update();
        return;
    }
    // 驱动游戏主帧更新
    GameManager::getInstance().frameUpdate(m_keyW, m_keyA, m_keyS, m_keyD, width(), height());
    // 刷新绘制
    update();
}

void GameCanvas::resetKeyState()
{
    m_keyLocked = false;
}

void GameCanvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // 调用分层绘制
    m_render.renderAll(&painter, width(), height());
}

void GameCanvas::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    auto& gameMgr = GameManager::getInstance();

    // WASD 移动（持续按下有效，不做防抖）
    switch (key)
    {
    case Qt::Key_W: m_keyW = true; break;
    case Qt::Key_A: m_keyA = true; break;
    case Qt::Key_S: m_keyS = true; break;
    case Qt::Key_D: m_keyD = true; break;
    }

    // 功能快捷键（防抖，防止连点）
    if (m_keyLocked&&!m_keyW&&!m_keyA&&!m_keyS&&!m_keyD)
        return;
    if(m_keyW||m_keyA||m_keyS||m_keyD||Qt::Key_Escape||Qt::Key_1||Qt::Key_2||Qt::Key_3||Qt::Key_T||Qt::Key_R){
        m_keyLocked = true;
        m_keyDebounceTimer->start();
    }

    GameGlobal::GameState curState = gameMgr.getGameState();
    switch (key)
    {
    // ESC 暂停/继续
    case Qt::Key_Escape:
        if (curState == GameGlobal::RUNNING)
            gameMgr.pauseGame();
        else
            gameMgr.resumeGame();
        break;

    // 1/2/3 躯体解构
    case Qt::Key_1:
        gameMgr.executeBodyDecompose(GameGlobal::DECOMPOSE_LIGHT);
        break;
    case Qt::Key_2:
        gameMgr.executeBodyDecompose(GameGlobal::DECOMPOSE_DEEP);
        break;
    case Qt::Key_3:
        gameMgr.executeBodyDecompose(GameGlobal::DECOMPOSE_FULL);
        break;

    // T 快速存档
    case Qt::Key_T:
        gameMgr.quickSaveGame();
        break;

    // R 时空回溯（回退上一节点）
    case Qt::Key_R:
        gameMgr.rollbackToLastNode();
        break;

    // F 启动环境拟态（持续5秒）
    case Qt::Key_F:
        gameMgr.startPlayerMimic(GameGlobal::MIMIC_ENV,5000);
        break;

    // G 触发基因熵变
    case Qt::Key_G:
        gameMgr.triggerPlayerGeneChaos();
        break;

    default:
        break;
    }
}

void GameCanvas::keyReleaseEvent(QKeyEvent *event)
{
    // 松开按键，清空移动标记
    switch (event->key())
    {
    case Qt::Key_W: m_keyW = false; break;
    case Qt::Key_A: m_keyA = false; break;
    case Qt::Key_S: m_keyS = false; break;
    case Qt::Key_D: m_keyD = false; break;
    }
}