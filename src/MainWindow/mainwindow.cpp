#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Lineage/LineageManager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_globalFrameCount(0)
{
    ui->setupUi(this);
    this->setWindowTitle("进化亿重奏：细胞纪元 V5.0");
    this->resize(1280, 720);
    LOG_INFO(MODULE_NAME, "主窗口创建完成，窗口尺寸 1280x720");

    // 1. 初始化渲染器，绑定窗口可视区域
    RenderCore::GetInstance()->InitRender(this->rect());

    // 2. 初始化生态体系
    EcologyCore::GetInstance()->InitEcology();

    // 3. 初始化交互系统
    InteractionManager::GetInstance()->InitInteraction();

    // 4. 初始化初始细胞群落
    InitCellGroup();

    // 5. 绑定对局结束事件
    connect(EventBus::GetInstance(), &EventBus::EVT_GAME_OVER,
            this, &MainWindow::OnGameOver);
    // 6. 启动60FPS全局定时器（16ms一帧）
    m_frameTimer = new QTimer(this);
    m_frameTimer->setInterval(16);
    connect(m_frameTimer, &QTimer::timeout, this, &MainWindow::OnGlobalFrameUpdate);
    m_frameTimer->start();
    LOG_INFO(MODULE_NAME, "60FPS全局帧循环定时器启动成功");
}

MainWindow::~MainWindow()
{
    // 停止定时器
    if (m_frameTimer->isActive())
        m_frameTimer->stop();
    delete m_frameTimer;
    LOG_INFO(MODULE_NAME, "全局定时器已停止并释放");

    // 批量释放所有细胞实例，防止内存泄漏
    qDeleteAll(m_cellList);
    m_cellList.clear();
    LOG_INFO(MODULE_NAME, "所有细胞实例内存已释放");

    delete ui;
}

void MainWindow::OnGlobalFrameUpdate()
{
    m_globalFrameCount++;

    // 第一步：全局生态更新
    EcologyCore::GetInstance()->EcologyUpdate(m_globalFrameCount);

    // 第二步：批量更新所有细胞AI
    int totalCell = m_cellList.size();
    for (Cell* cell : m_cellList)
    {
        if (cell == nullptr) continue;

        // 模拟参数：群落总数、随机生态位重叠度
        int groupCount = totalCell;
        double nicheOverlap = QRandomGenerator::global()->bounded(0, 1000)/10.0;
        bool isActive = true;

        cell->AIUpdate(m_globalFrameCount, isActive, groupCount, nicheOverlap);

        // 一级判定：单个细胞死亡，发布对局结束事件
        if (cell->CheckCellDeath())
        {
           /*
            QVariantList params;
            params << (int)Global::GameOverLevel::CellDeath;
            EventBus::GetInstance()->PublishEvent("EVT_GAME_OVER", params);*/
        }
    }

    // 第三步：驱动所有动态插件帧更新
    PluginManager::GetInstance()->UpdateAllPlugins(m_globalFrameCount);

    // 第四步：触发窗口重绘，执行渲染流程
    this->update();
}

void MainWindow::OnGameOver(Global::GameOverLevel level)
{
    QString overText;
    switch (level)
    {
    case Global::GameOverLevel::CellDeath:
        overText = "单个细胞死亡";
        break;
    case Global::GameOverLevel::GroupDeath:
        overText = "菌群全部消亡";
        break;
    case Global::GameOverLevel::HabitatCrash:
        overText = "局部生态崩溃";
        break;
    case Global::GameOverLevel::EcoTotalEnd:
        overText = "全域生态灭亡";
        LOG_INFO(MODULE_NAME, QString("对局判定结束，原因：全域生态灭亡，开始保存轮回数据并重置"));

        // 保存本轮数据（生存时长 = 总帧数 / 60）
        int surviveSec = m_globalFrameCount / 60;
        LineageManager::GetInstance()->SaveCurrentRound({}, surviveSec);

        // 重置本局，开启新轮回
        ResetCurrentRound();
        break;
    }
}

// 转发鼠标点击事件
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    InteractionManager::GetInstance()->OnMouseClick(event->pos(), event->button());
    QMainWindow::mousePressEvent(event);
}

// 转发鼠标移动事件
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    InteractionManager::GetInstance()->OnMouseMove(event->pos());
    QMainWindow::mouseMoveEvent(event);
}

// 转发键盘按键事件
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    InteractionManager::GetInstance()->OnKeyPress(event->key());
    QMainWindow::keyPressEvent(event);
}

// 窗口绘制事件
void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    RenderCore::GetInstance()->GlobalRender(&painter);
}

void MainWindow::InitCellGroup()
{
    // 清空旧细胞
    qDeleteAll(m_cellList);
    m_cellList.clear();
    RenderCore::GetInstance()->ClearRenderObjects();

    // 加载上一轮继承基因
    QVector<Global::Gene> inheritGenes = LineageManager::GetInstance()->LoadLastRoundGene();

    // 生成20个初始细胞群落
    const int cellSpawnCount = 20;
    for (int i = 0; i < cellSpawnCount; ++i)
    {
        Cell* cell = new Cell(this);
        // 随机出生坐标（限制在窗口内）
        QPointF spawnPos(
            QRandomGenerator::global()->bounded(50, 1230),
            QRandomGenerator::global()->bounded(50, 670)
            );
        // 初始层级1，模板ID=1
        cell->InitCell(1, 1, spawnPos, inheritGenes);

        m_cellList.append(cell);
        RenderCore::GetInstance()->AddCellObj(cell);
    }

    LOG_INFO(MODULE_NAME, QString("细胞群落初始化完成，生成数量：%1").arg(cellSpawnCount));
}

void MainWindow::ResetCurrentRound()
{
    InitCellGroup();
    m_globalFrameCount = 0;
    LOG_INFO(MODULE_NAME, "本局重置完成，进入新一轮轮回");
}