#ifndef DORO_H
#define DORO_H
#include <QPixmap>
#include <QRect>
#include <QElapsedTimer>
#include "enemy.h"
#include <QPainter>


class doro
{
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
        float GRAVITY = 0.6f;
        const float JUMP_FORCE = -15.0f;


        const float AIR_JUMP_MULTIPLIER = 1.0f;



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

};






#endif // DORO_H
