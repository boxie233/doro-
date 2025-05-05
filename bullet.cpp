// bullet.cpp
#include "bullet.h"
#include "config.h"


Bullet::Bullet(QPoint startPos, int type, QObject *parent): QObject(parent),SPEED(0), m_isDelayed(false)
{
    m_pixmap = QPixmap(BULLET_PATH).scaled(10, 20);

   if (type == 0) {
        sprite.load(BULLET_PATH);
       SPEED = 4;
        m_isDelayed = true;
        m_delayTimer.start();
        m_damage = 5;

   }
     else {
       sprite.load(BULLET_PATH2);
       SPEED=10;
       m_damage = 1;
   }


       rect = QRect(startPos.x() + 30, startPos.y() + 20,  sprite.width(), sprite.height());


}

void Bullet::update() {
      rect.translate(m_velocity.x(), m_velocity.y());

    if(m_isDelayed) {
        if(m_delayTimer.elapsed() < TIMETTT) return;
        m_isDelayed = false;
    }
    rect.moveTo(rect.x() + SPEED, rect.y());

    // 仅根据屏幕边界判断存活状态
    m_active = !(rect.x() > GAME_WIDTH + 200 || rect.x() + 200 < 0);
}




bool Bullet::isOutOfScreen() const {
    if(m_isDelayed) return false;
    return rect.x() > GAME_WIDTH;
}


QRect Bullet::getRect() const {
    return QRect(m_position.x(),m_position.y(),m_position.x(),m_position.x());
}

bool Bullet::isActive() const {
    return m_active;
}

void Bullet::setVelocity(const QPointF& velocity) {
    m_velocity = velocity;
}


QPointF m_velocity; // 新增速度成员变量
