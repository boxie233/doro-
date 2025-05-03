#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QTimer>
#include <QWidget>
#include "map.h"
#include "doro.h"
#include "bullet.h"
#include "config.h"
#include "enemy.h"

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
    int bulletCounts[2] = {ORANGENUMBER1,ORANGENUMBER2};
    QSet<int> m_pressedKeys;  // 新增：跟踪持续按下的键


    void handleContinuousInput();

    QList<Bullet*> m_bullets;  // 子弹容器
    void updateBullets();      // 新增子弹更新方法
    void cleanBullets();       // 清理溢出子弹



    void collisionDetection();

    int m_score=0;


    QList<Enemy*> m_enemies; // 新增敌人容器
    void spawnEnemy();       // 新增敌人生成方法
    void updateEnemies();    // 新增敌人更新方法

    bool m_stunBlinkState = false; // 新增闪烁状态
    QTimer m_stunBlinkTimer;       // 新增闪烁计时器

    void checkCharacterCollisions();

    static MainScene* instance(QWidget* parent = nullptr);



    static MainScene* m_instance;
    QList<Enemy*>& enemies();;

    float m_preDashSpeedX; // 记录冲刺前的水平速度
    bool m_wasGrounded;    // 记录冲刺前的地面状态

};
#endif // MAINSCENE_H
