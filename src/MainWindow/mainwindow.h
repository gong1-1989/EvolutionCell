#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include "Cell/Cell.h"
#include "EcologyCore/EcologyCore.h"
#include "Interaction/InteractionManager.h"
#include "Render/RenderCore.h"
#include "PluginManager/PluginManager.h"
#include "EventBus/EventBus.h"
#include "Global/GlobalDefine.h"
#include "Global/LogTool.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief 主窗口 + 全局60FPS主循环 【静态内核】
 * 架构说明：
 * 1. Qt窗口宿主、全局调度中枢，受Qt框架限制无法拆分为DLL
 * 2. 全局定时器 16ms ≈ 60FPS，统一驱动所有内核模块与插件
 * 3. 转发原始键鼠事件至交互模块，监听对局结束事件执行轮回重置
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    /**
     * @brief 全局60FPS帧更新主函数
     * 执行顺序：生态更新 → 细胞AI更新 → 插件更新 → 触发重绘
     */
    void OnGlobalFrameUpdate();

    /**
     * @brief 对局结束事件响应（四级对局判定）
     * @param level 对局结束等级
     */
    void OnGameOver(Global::GameOverLevel level);

protected:
    // 重写Qt原生事件，转发至交互模块
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::MainWindow *ui;
    QTimer* m_frameTimer;        // 全局60FPS定时器
    int m_globalFrameCount;      // 全局帧计数器（用于降频、节流）
    QVector<Cell*> m_cellList;   // 本局所有细胞实例容器

    /**
     * @brief 初始化本局细胞群落
     */
    void InitCellGroup();

    /**
     * @brief 重置本局（对局结束后，开启新一轮轮回）
     */
    void ResetCurrentRound();

    const QString MODULE_NAME = "主窗口&主循环";
};

#endif // MAINWINDOW_H