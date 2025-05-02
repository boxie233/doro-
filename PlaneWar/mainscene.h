#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QTimer>
#include <QWidget>
#include "map.h"
#include "doro.h"
#include "bullet.h"



class MainScene : public QWidget
{
    Q_OBJECT

public:
    MainScene(QWidget *parent = nullptr);
    ~MainScene();

    //初始化场景
    void initScene();

    void playGame();

    void updatePosition();

    void paintEvent(QPaintEvent *)override;;

    Map m_map;

    doro m_doro;


    void keyPressEvent(QKeyEvent* event)override; ;


    QTimer m_Timer;


    void keyReleaseEvent(QKeyEvent* event) ;

    int currentBulletType = 0;
    int bulletCounts[2] = {1000, 100};
    QSet<int> m_pressedKeys;  // 新增：跟踪持续按下的键


    void handleContinuousInput();

    QList<Bullet*> m_bullets;  // 子弹容器
    void updateBullets();      // 新增子弹更新方法
    void cleanBullets();       // 清理溢出子弹




};
#endif // MAINSCENE_H
