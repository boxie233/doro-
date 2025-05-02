// bullet.cpp
#include "bullet.h"
#include "config.h"

Bullet::Bullet(QPoint startPos, int type, QObject *parent)
    : QObject(parent),SPEED(0), m_isDelayed(false)
{
   if (type == 0) {
        sprite.load(BULLET_PATH);
       SPEED = 4;
        m_isDelayed = true;
        m_delayTimer.start();
   }
     else {
       sprite.load(BULLET_PATH2);
       SPEED=10;

   }
    rect = QRect(startPos.x() + 30, startPos.y() + 20,  sprite.width(), sprite.height());}

void Bullet::update() {
    if(m_isDelayed) {
               if(m_delayTimer.elapsed() < 800) {  return;  }
               m_isDelayed = false;
           }

    rect.moveTo(rect.x() + SPEED, rect.y());
}

bool Bullet::isOutOfScreen() const {
    if(m_isDelayed) return false;
    return rect.x() > GAME_WIDTH;
}
