#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <QObject>
#include <QTimer>
class GameLoop : public QObject
{
    Q_OBJECT
public:
    explicit GameLoop(QObject *parent = nullptr);
    void startLoop();
    void stopLoop();
signals:
    void frameUpdate(float deltaTime);
private slots:
    void onFrameTick();
private:
    QTimer* m_frameTimer;
    const int FRAME_RATE=60;
    float m_deltaTime;
};

#endif // GAMELOOP_H
