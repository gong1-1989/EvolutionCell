#ifndef RENDERCORE_H
#define RENDERCORE_H

#include <QObject>
#include <QPainter>
#include <QRect>
#include <QVector>
#include <QObject>
#include "Global/GlobalDefine.h"
#include "Global/LogTool.h"
#include "Cell/Cell.h"

/**
 * @brief 全局渲染核心 【静态内核 · 60FPS高频模块】
 * 架构说明：
 * 1. 渲染管线、图层分层、画布管理永久静态（跨DLL渲染会严重掉帧）
 * 2. 粒子特效、贴图、动画拓展拆分至 ParticlePlugin 动态插件
 * 视觉分层规则（自底向上7层）：
 * 1.背景层  2.地貌层  3.环境特效层  4.细胞本体层
 * 5.迁徙图标层  6.孢子动画层  7.UI弹窗层
 * 异常防护：绘制空对象、窗口越界、非法坐标 输出警告日志
 */
class RenderCore : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 获取全局单例
     */
    static RenderCore* GetInstance();

    /**
     * @brief 初始化渲染器
     * @param viewRect 主窗口可视区域
     */
    void InitRender(const QRect& viewRect);

    /**
     * @brief 全局统一绘制入口（主窗口paintEvent每帧调用）
     * @param painter 主画布绘制器
     */
    void GlobalRender(QPainter* painter, const QVector<Cell *> &cellList);

    /**
     * @brief 清空所有绘制对象（新轮回/场景重置调用）
     */
    void ClearRenderObjects();

    // ===================== 分层注册绘制对象 =====================
    void AddBackgroundObj(const QRect& obj);
    void AddLandformObj(const QRect& obj);
    //void AddCellObj(QObject* cell);//已废止，改为实时同步主列表

private:
    RenderCore() = default;
    static RenderCore* m_instance;
    const QString MODULE_NAME = "渲染核心";

    QRect m_viewRect;                // 主窗口可视范围
    QVector<QRect> m_bgLayer;        // 1.背景层
    QVector<QRect> m_landLayer;       // 2.地貌层
    //QVector<QObject*> m_cellLayer;   // 4.细胞本体层（包含动画、图标）--废止
};

#endif // RENDERCORE_H