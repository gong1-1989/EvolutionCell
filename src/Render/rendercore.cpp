#include "RenderCore.h"
#include "Cell/Cell.h"

RenderCore* RenderCore::m_instance = nullptr;

RenderCore* RenderCore::GetInstance()
{
    static QMutex instMutex;
    QMutexLocker locker(&instMutex);

    if (m_instance == nullptr)
    {
        m_instance = new RenderCore;
        LOG_INFO(m_instance->MODULE_NAME, "渲染核心单例创建成功");
    }
    return m_instance;
}

void RenderCore::InitRender(const QRect& viewRect)
{
    m_viewRect = viewRect;
    ClearRenderObjects();
    LOG_INFO(MODULE_NAME, QString("渲染器初始化完成，可视区域：%1x%2")
                              .arg(viewRect.width()).arg(viewRect.height()));
}

void RenderCore::GlobalRender(QPainter* painter)
{
    // 空绘制器防护
    if (painter == nullptr)
    {
        LOG_ERR(MODULE_NAME, "绘制器指针为空，跳过本帧渲染");
        return;
    }

    painter->save();
    // 开启抗锯齿，优化绘制效果
    painter->setRenderHint(QPainter::Antialiasing, true);

    // ========== 1. 绘制背景层 ==========
    painter->setBrush(QColor(20, 30, 40));
    painter->setPen(Qt::NoPen);
    painter->drawRect(m_viewRect);

    // ========== 2. 绘制地貌层 ==========
    painter->setBrush(QColor(40, 50, 60));
    for (const QRect& rect : m_landLayer)
    {
        // 坐标越界检测
        if (!m_viewRect.intersects(rect))
        {
            LOG_DBG(MODULE_NAME, "地貌图形超出可视区域，跳过绘制");
            continue;
        }
        painter->drawRect(rect);
    }

    // ========== 4. 绘制细胞层（本体+迁徙图标+孢子动画） ==========
    for (QObject* obj : m_cellLayer)
    {
        // 空对象防护
        if (obj == nullptr)
        {
            LOG_WARN(MODULE_NAME, "细胞层存在空对象，跳过绘制");
            continue;
        }

        // 类型转换
        Cell* cell = qobject_cast<Cell*>(obj);
        if (cell == nullptr)
        {
            LOG_WARN(MODULE_NAME, "对象非细胞实例，跳过绘制");
            continue;
        }

        cell->Draw(painter);
    }

    painter->restore();
}

void RenderCore::ClearRenderObjects()
{
    m_bgLayer.clear();
    m_landLayer.clear();
    m_cellLayer.clear();
    LOG_DBG(MODULE_NAME, "所有绘制对象已清空");
}

void RenderCore::AddBackgroundObj(const QRect& obj)
{
    m_bgLayer.append(obj);
}

void RenderCore::AddLandformObj(const QRect& obj)
{
    m_landLayer.append(obj);
}

void RenderCore::AddCellObj(QObject* cell)
{
    if (cell == nullptr)
    {
        LOG_WARN(MODULE_NAME, "尝试添加空细胞对象到渲染队列，已拦截");
        return;
    }
    m_cellLayer.append(cell);
}