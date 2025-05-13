#include "enemy.h"
#include "config.h"
#include "doro.h"
#include <QUrl>  // 处理 URL 需要的头文件


Enemy::Enemy(QPoint position, Type type) : position(position),enemyType(type) {


    if(enemyType == Type::Boss) {

        spriteRight.load(BOSS_PATH_RIGHT);
        spriteLeft.load(BOSS_PATH_LEFT);
        speed = BOSS_SPEED;
        health = BOSS_HEALTH;
        m_canJump = true;
        m_jumpTimer.start();

        m_jumpForce = BOSS_JUMP_FORCE;
        m_shieldHealth = 100;
        m_isShieldActive = false;
        m_dashSpeed = 300.0f;
        m_dashDuration = 0.3f;

        m_cooldownTimer.start();


    }
    else if(enemyType == Type::Flying) {
        spriteRight.load(FLYING_ENEMY_PATH);
        spriteLeft.load(FLYING_ENEMY_PATH); // 飞行敌人可以只用单方向精灵
        speed = FLYING_ENEMY_SPEED;
        health = FLYING_ENEMY_HEALTH;

        // 初始化随机移动参数
        velocity = QPoint(QRandomGenerator::global()->bounded(-2, 3), QRandomGenerator::global()->bounded(-2, 3));
        directionTimer.start();
    } else {
        // 原有地面敌人初始化

        spriteRight.load(ENEMY_PATH_RIGHT);
        spriteLeft.load(ENEMY_PATH_LEFT);
        speed = ENEMY_SPEED;
        health = ENEMY_HEALTH;


    }


}
bool Enemy::isShieldActive()  {
    return m_isShieldActive;  // 直接返回标志位
}



void Enemy::activateShield() {
    spriteRight.load(BOSS_PATH_RIGHT);
    spriteLeft.load(BOSS_PATH_LEFT);
    m_isShieldActive = true;

}

void Enemy::deactivateShield() {
    shieldSpriteRight.load(SHIELD_RIGHT_SPRITE_PATH);  // 加载右向护盾
    shieldSpriteLeft.load(SHIELD_LEFT_SPRITE_PATH);    // 加载左向护盾
    m_isShieldActive = false;

}

void Enemy::startSkillCooldown() {
    if(enemyType == Type::Boss) {
        skillTimer.start(BOSS_SKILL_COOLDOWN);
    }
}

void Enemy::update() {

    if (enemyType == Type::Boss) {
        // 自动跳跃逻辑
        if (m_jumpTimer.elapsed() >= BOSS_JUMP_INTERVAL) {
            m_velocityY = BOSS_JUMP_FORCE;  // 使用成员变量
            m_jumpTimer.restart();
            m_isJumping = true;


         if (m_cooldownTimer.elapsed() >= m_shieldCooldown && !m_isShieldActive) {
           m_cooldownTimer.restart();
         }

         if (m_cooldownTimer.elapsed() >= m_dashCooldown) {

             m_cooldownTimer.restart();
         }

         // 执行冲刺
         if (m_isDashing) {
             m_velocityX = m_dashDirection * m_dashSpeed;
             m_dashTimer.start();

             if (m_dashTimer.elapsed() >= m_dashDuration * 1000) {
                 m_isDashing = false;
                 m_velocityX = 0;
             }
         }

        }

        // 自动触发护盾条件
        if (!m_isShieldActive && health < BOSS_HEALTH * 0.80) {
         activateShield();
         }

            // 处理护盾持续时间
        if (m_isShieldActive) {
          if (m_shieldTimer.elapsed() >= m_shieldDuration) {
           deactivateShield();
        }
        }


        // 应用重力
        m_velocityY += BOSSGRAVITY;
        position.ry() += m_velocityY;

        // 地面碰撞检测
        int bossHeight = spriteRight.height()-160;
        if (position.y() > BOSS_GROUND_LEVEL - bossHeight) {
            position.setY(BOSS_GROUND_LEVEL - bossHeight);
            m_velocityY = 0;
            m_isJumping = false;
        }
    }

  if(enemyType == Type::Flying) {
        // 自由飞行逻辑
        position += velocity;

        // 每1.5秒随机改变方向
        if(directionTimer.elapsed() > 1500) {
            velocity = QPoint(QRandomGenerator::global()->bounded(-2, 3), QRandomGenerator::global()->bounded(-2, 3));
            directionTimer.restart();
        }

        // 屏幕边界反弹
        if(position.x() < 0 || position.x() > GAME_WIDTH - spriteRight.width()) {
            velocity.setX(-velocity.x());
        }
        if(position.y() < 50 || position.y() > GAME_HEIGHT - 100) {
            velocity.setY(-velocity.y());
        }
    } else {
        // 原有地面敌人移动逻辑
        position.rx() += (facingRight ? speed : -speed);

        if(position.x() > GAME_WIDTH + 20) facingRight = false;
        if(position.x() < 20) facingRight = true;
    }
}

QRect Enemy::getRect() const {

    if(enemyType == Type::Boss) {
        return QRect(position.x() + 20, position.y() + 22, spriteRight.width()-40, spriteRight.height()-50);
    }
    else if(enemyType == Type::Flying) {
        // 飞行敌人碰撞框
        return QRect(position.x() + 15, position.y() + 15,  spriteRight.width() - 30, spriteRight.height() - 30);
    }
    // 原有地面敌人碰撞框
    return QRect(position.x() + spriteRight.width()*0.1,  position.y() + spriteRight.height()*0.1 + 10, spriteRight.width()*0.8,  spriteRight.height()*0.9 - 20);
}


void Enemy::takeDamage(int damage) {
    if (m_isShieldActive) {
        m_shieldHealth -= damage;
        if (m_shieldHealth <= 0) {
            m_shieldHealth = 0;
            deactivateShield();
        }
    } else {
        health -= damage;
        health = qMax(0, health);
    }


}

bool Enemy::isAlive() const {
    return health > 0 && position.y() < GAME_HEIGHT; // 添加坠落检测
}

QRect Enemy::getSpriteRect() const {
    // 根据当前方向返回精灵的矩形区域
    QPixmap currentSprite = facingRight ? spriteRight : spriteLeft;
    return QRect(position, currentSprite.size());
}


void Enemy::checkDashCondition(const QPoint& playerPos) {

}
