#ifndef DORO_H
#define DORO_H
#include <QPixmap>
#include <QRect>
#include <QElapsedTimer>
#include "enemy.h"
#include <QPainter>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTimer>
#include <QObject>
class doro  {

public:
    doro();

    void shoot();

    void setPosition(int x,int y);

    QPixmap m_doro;

    int m_X;
    int m_Y;

    QRect m_Rect;


    void moveLeft();
    void moveRight();
    void jump();

    // 新增物理属性
    void updatePhysics();
    void resetJumps();
    int m_availableJumps = 2;
    const int MAX_JUMPS = 3;
    bool m_isGrounded = true;
    float m_velocityY = 0;
    float GRAVITY = 0.3f;
    const float JUMP_FORCE = -13.0f;
    const float AIR_JUMP_MULTIPLIER = 0.8f;
    void startStun(int duration);
    bool isStunned() const { return m_isStunned; }
    bool m_isStunned = false;
    QElapsedTimer m_stunTimer;
    int m_stunDuration = 0;

    QPixmap m_doroRight;  // 新增向右图标
    QPixmap m_doroLeft;   // 新增向左图标
    bool m_facingRight = true; // 新增方向状态

    Enemy* standingEnemy = nullptr; // 新增站立目标
    float standingOffsetX = 0;      // 相对敌人X轴偏移


    QRect getFeetRect() const {
        return QRect(m_X + m_Rect.width()*0.2f, m_Y + m_Rect.height()*0.8f+20, m_Rect.width()*0.6f, m_Rect.height()*0.2f);
    }



    bool m_isDashing;
    float m_dashProgress;
    int m_dashDirection;
    QElapsedTimer m_dashCooldownTimer;
    QList<QPixmap> m_dashAfterimages;
    void startDash();
    void updateDash();
    void addAfterimage();

    void checkDashCollision();


    float m_velocityX = 0;

    bool isDashing() const { return m_isDashing; }

    const QList<QPixmap>& getAfterimages() const { return m_dashAfterimages; }
    int dashDirection() const { return m_dashDirection; }


    float m_preDashSpeedX; // 记录冲刺前的水平速度
    bool m_wasGrounded;    // 记录冲刺前的地面状态
    float m_preDashSpeedY;


    bool m_isOnEnemy = false;
    bool isOnEnemy() const { return m_isOnEnemy; }  // 新增敌人站立状态


    // 保持原有二段跳相关变量
    int m_remainingJumps = 2;

    // 新增方法
    void toggleForm(); // 切换形态
    void verticalMove(int direction); // 垂直移动控制（1:上, -1:下）

    // 修改后的成员变量
    bool m_secondForm = false; // 形态标识
    QPixmap m_form2Right;      // 第二形态右向
    QPixmap m_form2Left;       // 第二形态左向
    float m_verticalSpeed = 0; // 垂直移动速度

    bool isSecondForm() const { return m_secondForm; }



    QPixmap m_form3Right;  // 形态3右向图片
    QPixmap m_form3Left;   // 形态3左向图片
    void enterForm3();  // 进入形态3
    void exitForm3();   // 退出形态3
    bool isForm3Active() const { return !m_secondForm && m_wasSecondForm; }
    bool m_wasSecondForm = false;  // 新增变量，用于保存切换形态3前的状态

    int health = 100;

    void activateShield(int shieldHP = 100);  // 激活护盾
    void updateShield();                      // 更新护盾状态
    bool isShieldActive() const { return m_isShieldActive; }
    int getShieldHP() const { return m_shieldHP; }

    bool m_isShieldActive = false;          // 护盾激活状态
    int m_shieldHP = 0;                     // 当前护盾值
    int m_maxShieldHP = 100;                // 最大护盾值
        // 护盾持续时间计时器
    void takeDamage(int damage);


    QTimer m_shieldTimer;
    int getMaxShieldHP() const { return m_maxShieldHP; }


    bool maodie=false;



};






#endif // DORO_H
