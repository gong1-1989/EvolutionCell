#ifndef GAMEMAIN_H
#define GAMEMAIN_H

#include <QKeyEvent>
#include <QList>
#include <QPainter>
#include <QRandomGenerator>
#include <QWidget>
#include "Cell/cellentity.h"
#include "Cell/gentcell.h"
#include "Cell/monsterentity.h"
#include "Core/gameloop.h"
#include "Utils/collisionutil.h"
#include "Utils/levelsystem.h"

class GameMain : public QWidget
{
    Q_OBJECT
public:
    GameMain(QWidget *parent = nullptr);
    ~GameMain() = default;

private:
    LevelSystem* m_levelSys;
    Cellentity *m_player;
    QList<MuscleCell> m_muscleCells;
    QList<ShieldCell> m_shieldCells;
    QList<MagicCell> m_magicCells;
    QList<MonsterEntity> m_monsters;
    GameLoop *m_gameLoop;
    bool m_keyW = false;
    bool m_keyS = false;
    bool m_keyA = false;
    bool m_keyD = false;
    QTimer *m_spawnTimer;
    void movePlayer(float detaTime);
protected:
    void paintEvent(QPaintEvent *e) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
private slots:
    void UpdateGame(float deltaTime);
    void spawnMonster();
};

#endif // GAMEMAIN_H
