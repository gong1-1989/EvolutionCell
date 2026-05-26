#ifndef GAMERENDER_H
#define GAMERENDER_H

#include <QPainter>
#include "Core/GameManager.h"
#include "Core/GameGlobal.h"
#include "Cell/PlayerCell.h"
#include "Cell/MonsterCell.h"
#include "Cell/SymbiosisCell.h"
#include "Cell/GhostCell.h"
#include <QPainter>
#include <QBrush>
#include <QPen>


/**
 * @brief 游戏绘制器
 * 绘制层级（从底层到顶层，严格顺序）：
 * 1. 背景  2. 怪物  3. 共生菌体  4. 时空残影  5. 玩家主体  6. HUD文字
 * 优化：抽取通用圆形绘制函数、统一颜色/布局、使用全局常量
 */
class GameRender
{
public:
    GameRender() = default;

    /**
     * @brief 全场景绘制入口
     * @param painter 绘图上下文
     * @param canvasW 画布宽度
     * @param canvasH 画布高度
     */
    void renderAll(QPainter* painter, int canvasW, int canvasH);

private:
    // 通用工具：绘制圆形细胞
    void drawCircleCell(QPainter* painter, qreal x, qreal y, int size, const QColor& color, bool transparent = false);

    // 分层绘制原子函数
    void drawBackground(QPainter* painter, int canvasW, int canvasH);
    void drawMonster(QPainter* painter);
    void drawSymbiosis(QPainter* painter);
    void drawGhost(QPainter* painter);
    void drawPlayer(QPainter* painter);
    void drawHUD(QPainter* painter);
};

#endif // GAMERENDER_H