// bullettype.h
#pragma once
#include <QObject>
#include <QPixmap>

class BulletBase : public QObject {
    Q_OBJECT
public:
    enum BulletType { Normal, Fire, Ice };

    BulletBase(QPoint startPos, BulletType type, QObject* parent = nullptr);
    virtual void update() = 0;
    bool isOutOfScreen() const;

    QPixmap sprite;
    QRect rect;
    BulletType bulletType;
    int damage = 1;

protected:
    int m_speed;
};

class NormalBullet : public BulletBase {
public:
    NormalBullet(QPoint startPos)
        : BulletBase(startPos, BulletType::Normal) {
        m_speed = 12;
        sprite.load(":/res/bullet_normal.png");
    }
    void update() override {
        rect.moveTo(rect.x() + m_speed, rect.y());
    }
};

class FireBullet : public BulletBase {
public:
    FireBullet(QPoint startPos)
        : BulletBase(startPos, BulletType::Fire) {
        m_speed = 15;
        damage = 2;
        sprite.load(":/res/bullet_fire.png");
    }
    void update() override {
        rect.moveTo(rect.x() + m_speed, rect.y() - 2); // 火焰弹抛物线
    }
};

class IceBullet : public BulletBase {
public:
    IceBullet(QPoint startPos)
        : BulletBase(startPos, BulletType::Ice) {
        m_speed = 10;
        sprite.load(":/res/bullet_ice.png");
    }
    void update() override {
        rect.moveTo(rect.x() + m_speed, rect.y());
    }
};
