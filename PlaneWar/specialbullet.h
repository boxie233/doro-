// 新建 specialbullet.h
#pragma once
#include "bullet.h"

class NormalBullet : public Bullet {
public:
    NormalBullet(QPoint pos)
        : Bullet(pos, Bullet::Normal) {
        speed = 12;
        sprite.load(":/res/bullet_normal.png");
    }
    void update() override {
        rect.moveTo(rect.x() + speed, rect.y());
    }
};

class FireBullet : public Bullet {
public:
    FireBullet(QPoint pos)
        : Bullet(pos, Bullet::Fire) {
        speed = 18;
        damage = 2;
        sprite.load(":/res/bullet_fire.png");
    }
    void update() override {
        rect.moveTo(rect.x() + speed, rect.y() - 2);
    }
};

class IceBullet : public Bullet {
public:
    IceBullet(QPoint pos)
        : Bullet(pos, Bullet::Ice) {
        speed = 10;
        sprite.load(":/res/bullet_ice.png");
    }
    void update() override {
        rect.moveTo(rect.x() + speed, rect.y() + 1);
    }
};
