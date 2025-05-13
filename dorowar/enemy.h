#ifndef ENEMY_H
#define ENEMY_H
#include "config.h"
#include <QPixmap>
#include <QRect>
#include <QTimer>
#include <QElapsedTimer>
#include <QRandomGenerator>
#include <QMediaPlayer>  // 添加多媒体类声明
#include <QUrl>          // 如果需要处理URL
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QMediaPlayer>  // 必须包含头文件




class Enemy {
public:


    enum Type { Ground, Flying, Boss };  // 添加Boss类型
      Type enemyType;
    // 在类中添加以下成员
    bool m_canJump = false;
    float m_jumpForce = 0;
    QElapsedTimer m_jumpTimer;




    Enemy(QPoint position,Type type = Type::Ground);
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

    QPoint velocity; // 用于自由移动的向量
    QElapsedTimer directionTimer; // 方向变化计时器

    bool isGrounded() const {
        return (enemyType == Type::Boss) ? (position.y() >= BOSS_GROUND_LEVEL) : (position.y() >= GAME_HEIGHT - spriteRight.height());
    }


    bool m_isJumping = false;

    float m_velocityY = 0;  // 新增垂直速度变量

    void startSkillCooldown();
    bool tryCastSkill();
    QTimer skillTimer;
    bool isCasting = false;
    qint64 lastSkillTime = 0;

    const int BOSS_SKILL_COOLDOWN = 10000; // 10秒冷却


    void executeSkill();

     QRect getSpriteRect() const;

    bool m_isShieldActive=false;       // 护盾激活状态
    int m_shieldDuration = 5000; // 护盾持续时间（5秒）
    int m_shieldCooldown = 10000;// 护盾冷却时间（10秒）
    int m_shieldHealth = 100;    // 护盾耐久值
    QElapsedTimer m_shieldTimer; // 护盾持续时间计时器
    QElapsedTimer m_cooldownTimer; // 护盾冷却计时器

    void activateShield();
    void deactivateShield();

    QPixmap shieldSpriteRight;  // 右向护盾精灵
    QPixmap shieldSpriteLeft;   // 左向护盾精灵


    bool isShieldActive() ;
    QPixmap originalSpriteRight; // 原始右向精灵（保存初始状态）
    QPixmap originalSpriteLeft;  // 原始左向精灵


    bool m_isDashing = false;
    float m_dashSpeed = 200.0f;  // 冲刺速度
    float m_dashDuration = 0.5f; // 冲刺持续时间（秒）
    QElapsedTimer m_dashTimer;    // 冲刺计时器

    // 新增冲刺触发条件
    void checkDashCondition();

    float m_dashCooldown = 2000;
    float m_velocityX = 0;  // 新增水平速度变量
int m_dashDirection = 1;
   void checkDashCondition(const QPoint& playerPos);

};





#endif // ENEMY_H
