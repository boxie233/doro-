#ifndef ENEMY_H
#define ENEMY_H
#include "config.h"
#include <QPixmap>
#include <QRect>
#include <QTimer>
class Enemy {
public:
    Enemy(QPoint position);
    void update();
    void takeDamage(int damage);

    QRect getRect() const;
    bool isAlive() const;

    QPoint position;
    int speed = ENEMY_SPEED;
    int health = ENEMY_HEALTH;
    bool facingRight = true;

    QPixmap spriteRight;
    QPixmap spriteLeft;
    void changeDirection();


};

#endif // ENEMY_H
