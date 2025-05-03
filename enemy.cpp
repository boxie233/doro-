#include "enemy.h"
#include "config.h"

Enemy::Enemy(QPoint position) : position(position) {
    spriteRight.load(ENEMY_PATH_RIGHT);
    spriteLeft.load(ENEMY_PATH_LEFT);
}

void Enemy::update() {
    // 水平移动
    position.rx() += (facingRight ? speed : -speed);

    // 简单边界检测
    if(position.x() > GAME_WIDTH+20 ) {
        facingRight = 0;
    }

    if(position.x() < 20 ) {
        facingRight = 1;
    }


}




QRect Enemy::getRect() const {
    return QRect( position.x() + spriteRight.width()*0.1,position.y() + spriteRight.height()*0.1+10,spriteRight.width()*0.8,spriteRight.height()*0.9-20);
}



void Enemy::takeDamage(int damage) {
    health -= damage;
    health = qMax(0, health); // 确保生命值不低于0
}

bool Enemy::isAlive() const {
    return health > 0 && position.y() < GAME_HEIGHT; // 添加坠落检测
}


