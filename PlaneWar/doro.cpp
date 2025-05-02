#include "doro.h"
#include "config.h"
#include <QtCore/qglobal.h>

doro::doro() {

    m_doro.load(DORO_PATH);

    m_Y =( GAME_HEIGHT-m_doro.height()-50);
    m_X =( m_doro.width()-50);

    m_Rect.setWidth(m_doro.width());
    m_Rect.setHeight(m_doro.height());
    m_Rect.moveTo(m_X,m_Y);

}



void doro::setPosition(int x,int y){
    m_X=x;
    m_Y=y;

    m_Rect.moveTo(m_X,m_Y);

};


void doro::shoot(){};

void doro::moveLeft() {
    m_X = qMax(0, m_X - MOVE_SPEED);
    m_Rect.moveTo(m_X, m_Y);
}

void doro::moveRight() {
    int maxX = GAME_WIDTH - m_doro.width();
    m_X = qMin(maxX, m_X + MOVE_SPEED);
    m_Rect.moveTo(m_X, m_Y);
}



void doro::jump() {
    if(m_availableJumps > 0) {
        float actualForce = (m_availableJumps == MAX_JUMPS)
        ? JUMP_FORCE
        : JUMP_FORCE * AIR_JUMP_MULTIPLIER;

        m_velocityY = actualForce;
        m_isGrounded = false;
        m_availableJumps--;
    }
}


void doro::updatePhysics() {

    if (!m_isStunned) {
        // 正常物理计算
        m_velocityY += GRAVITY;
        m_Y += m_velocityY;

        // 地面检测
        int groundLevel = GAME_HEIGHT - m_doro.height() - 50;
        if (m_Y >= groundLevel) {
            m_Y = groundLevel;
            m_velocityY = 0;
            m_isGrounded = true;
            resetJumps();
        }
    }
    m_Rect.moveTo(m_X, m_Y);
}



void doro::resetJumps() {
    m_availableJumps = MAX_JUMPS; // 重置为最大跳跃次数
}





void doro::startStun(int duration) {
    m_isStunned = true;
    m_stunDuration = duration;
    m_stunTimer.start();
    m_velocityY = 0; // 清除下落速度
}
