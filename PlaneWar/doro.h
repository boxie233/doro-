#ifndef DORO_H
#define DORO_H
#include <QPixmap>
#include <QRect>
#include <QElapsedTimer>



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
        const float GRAVITY = 0.8f;
        const float JUMP_FORCE = -20.0f;
        const float MOVE_SPEED = 5.0f;

        const float AIR_JUMP_MULTIPLIER = 1.0f;



        void startStun(int duration);
        bool isStunned() const { return m_isStunned; }

        bool m_isStunned = false;
        QElapsedTimer m_stunTimer;
        int m_stunDuration = 0;


};



void startStun(int duration);


#endif // DORO_H
