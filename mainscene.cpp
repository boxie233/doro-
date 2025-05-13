#include "mainscene.h"
#include "config.h"
#include <QIcon>
#include <QPainter>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QSoundEffect>
#include <QMessageBox>
#include <QApplication>  // 添加这行

MainScene::MainScene(QWidget *parent) : QWidget(parent), m_enemySpawnTimer(this)
{

    m_bossTextTimer.setSingleShot(true); // 单次触发
    connect(&m_bossTextTimer, &QTimer::timeout, [this](){
        m_bossTextVisible = false;
        update(); // 触发界面更新
    });

    initScene();//调用初始化场景

    playGame();

    m_bullets.reserve(50);
    spawnEnemy();

    m_stunBlinkTimer.setInterval(500); // 闪烁间隔
    connect(&m_stunBlinkTimer, &QTimer::timeout, [this](){
        m_stunBlinkState = !m_stunBlinkState;
        update();
    });

    m_enemies.append(new Enemy(QPoint(300, GROUND_Y))); // 固定位置敌人


    connect(&m_enemySpawnTimer, &QTimer::timeout, this, &MainScene::spawnEnemy);
    m_enemySpawnTimer.start(ENEMY_SPAWN_INTERVAL);

    QSoundEffect shieldSound;
    shieldSound.setSource(QUrl("qrc:/res/shield_activate.wav"));
}


MainScene::~MainScene() {
}

void MainScene::initScene(){

    //设置窗口尺寸

    setFixedSize(GAME_WIDTH,GAME_HEIGHT);

    //设置标题

    setWindowTitle(GAME_TITLE);

    setWindowIcon(QIcon(GAME_ICON));

    m_Timer.setInterval(GAME_RATE);
}

void MainScene::playGame(){m_Timer.start();connect(&m_Timer,&QTimer::timeout,[=](){

        if (m_score >= SCORE )
            gameWin("胜利");

        handleContinuousInput();

        m_doro.updatePhysics(); // 新增物理更新

        updatePosition();

        update();

        updateBullets();    // 更新子弹位置
        cleanBullets();     // 清理溢出子弹

        updateEnemies();    // 更新敌人状态


        checkCharacterCollisions(); // 添加角色与敌人的碰撞检测
        collisionDetection();       // 原有的子弹碰撞检测

        if(m_doro.isDashing()) {  // 现在调用的是公共方法
            m_doro.updateDash();
        } else {
            handleContinuousInput();
            m_doro.updatePhysics();
        }
    });
};

void MainScene::handleContinuousInput() {
    if (m_doro.isStunned()) return; // 定身期间禁止移动


    // 当站在敌人上时允许移动
    if(m_pressedKeys.contains(Qt::Key_A)) {
        m_doro.moveLeft();
        if(m_doro.standingEnemy) {
            // 更新相对偏移量
            m_doro.standingOffsetX -= MOVE_SPEED;
        }
    }
    if (m_pressedKeys.contains(Qt::Key_D)) {
        m_doro.moveRight();
    }

    // 左右移动处理
    if(m_pressedKeys.contains(Qt::Key_A)) {
        m_doro.moveLeft();
    }
    if(m_pressedKeys.contains(Qt::Key_D)) {
        m_doro.moveRight();
        if(m_doro.standingEnemy) {
            m_doro.standingOffsetX += MOVE_SPEED;
        }
    }

    // 跳跃处理（瞬时触发）
    if(m_pressedKeys.contains(Qt::Key_Space)) {
        m_doro.jump();
        m_pressedKeys.remove(Qt::Key_Space);  // 防止持续触发
    }


}

void MainScene::updatePosition(){
    m_map.mapPosition();

}

void MainScene::paintEvent(QPaintEvent *){
    QPainter painter(this);


    painter.drawPixmap(m_map.m_map1_posX,0,m_map.m_map1);
    painter.drawPixmap(m_map.m_map2_posX,0,m_map.m_map2);




    for(const auto bullet : m_bullets) {
        painter.drawPixmap(bullet->rect.topLeft(), bullet->sprite);
    }

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 10));
    painter.setPen(QColor(0, 150, 255));  // 亮蓝色文字
    QString ammoText = QString("欧润橘%1: %2只").arg(currentBulletType + 1).arg(bulletCounts[currentBulletType]);
    painter.drawText(20, 40, ammoText);
    painter.setPen(QColor(150, 255, 0));
    painter.drawText(20, 70, QString("击杀分: %1").arg(m_score));


    if (m_doro.isStunned()) {
        painter.setPen(Qt::red);
        painter.setFont(QFont("Arial", 70, QFont::Bold));
    }



    for(const auto enemy : m_enemies) {
        QPixmap currentSprite = enemy->facingRight ?  enemy->spriteRight : enemy->spriteLeft;
        painter.drawPixmap(enemy->position, currentSprite);
    }




    if (m_doro.m_isStunned) {
        if(!m_stunBlinkTimer.isActive()) {
            m_stunBlinkTimer.start();
        }

        if(m_stunBlinkState) {
            painter.setBrush(QColor(255, 0, 0, 80));
            painter.setPen(Qt::NoPen);
            painter.drawRect(rect());
            QFont font("Arial", 72, QFont::Bold);
            painter.setFont(font);
            painter.setPen(QColor(0, 0, 0, 150));
            painter.drawText(GAME_WIDTH/2-200, GAME_HEIGHT/2 , "奥义*欧润橘");
            painter.setPen(QColor(255, 255, 0));
            painter.drawText(GAME_WIDTH/2-200, GAME_HEIGHT/2, "奥义*欧润橘");
            QPixmap stunIcon(DORO_PATH);
            painter.drawPixmap(GAME_WIDTH/2-400, GAME_HEIGHT/2-170,200, 200, stunIcon);
        }
    }
    else {
        if(m_stunBlinkTimer.isActive()) {
            m_stunBlinkTimer.stop();
            m_stunBlinkState = false;
        }
    }

    if(m_doro.isDashing()) {
        const auto& afterimages = m_doro.getAfterimages();
        float opacityStep = 0.8f / afterimages.size();
        for(int i=0; i<afterimages.size(); ++i) {
            painter.setOpacity(0.8f - i*opacityStep);
            painter.drawPixmap(
                m_doro.m_X - (afterimages.size()-i-1)*30*m_doro.dashDirection(),
                m_doro.m_Y,
                afterimages[i]
                );
        }
        painter.setOpacity(1.0f);
    }



    foreach (Enemy* enemy, m_enemies) {
        if (enemy->enemyType == Enemy::Type::Boss) {
            QPixmap currentSprite = enemy->facingRight ? enemy->shieldSpriteRight : enemy->shieldSpriteLeft;

            painter.drawPixmap(enemy->position, currentSprite);
        } else {
            // 绘制其他敌人（保持原有逻辑）
            QPixmap currentSprite = enemy->facingRight? enemy->spriteRight  : enemy->spriteLeft;
            painter.drawPixmap(enemy->position, currentSprite);
        }
    }

    if (m_bossTextVisible) {
        painter.setPen(QColor(0, 150, 255));  // 亮蓝色文字
        QFont font("Arial", 24, QFont::Bold);
        painter.setFont(font);

        // 获取文字尺寸
        QFontMetrics fm(font);
        int textHeight = fm.height();
        int textWidth = fm.horizontalAdvance("BOSS 出现！");

        // 计算背景框位置
        int padding = 10; // 内边距
        int boxTop = (height() - textHeight) / 2 - padding;
        int boxHeight = textHeight + padding * 2;

        // 绘制背景框（仅垂直方向包裹文字）
        painter.setBrush(QColor(0, 0, 50, 200)); // 深蓝背景
        painter.drawRect(0, boxTop, width(), boxHeight);

        // 绘制文字（保持居中）
        painter.drawText((width() - textWidth)/2, (height() + textHeight)/2 - padding,"BBBBBBIG DORO 出现！");
    }



    QPixmap currentSprite;
    if(m_doro.isSecondForm()) {
        currentSprite = m_doro.m_facingRight ? m_doro.m_form2Right : m_doro.m_form2Left;
    } else {
        currentSprite = m_doro.m_facingRight ?m_doro.m_doroRight : m_doro.m_doroLeft;
    }
    painter.drawPixmap(m_doro.m_X, m_doro.m_Y, currentSprite);

    foreach (Enemy* enemy, m_enemies) {
        if (enemy->isShieldActive()) {  // 检查护盾是否激活
            QPixmap shieldSprite = enemy->facingRight ? enemy->shieldSpriteRight: enemy->shieldSpriteLeft;
            painter.drawPixmap(enemy->position, shieldSprite);

        }
    }

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 14));
    painter.setPen(QColor(255, 0, 0));  // 亮蓝色文字
    painter.drawText(200, 50, QString("生命：%1").arg(m_doro.health));




    if (m_doro.isShieldActive()) {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 14, QFont::Bold));
        painter.drawText(GAME_WIDTH - 150, 30, QString("护盾值: %1/%2").arg(m_doro.getShieldHP()).arg(m_doro.getMaxShieldHP()));

    }




};

void MainScene::keyPressEvent(QKeyEvent* event) {

    if (event->key() == Qt::Key_F && !event->isAutoRepeat()) {
        m_doro.activateShield();
    }

    if (event->key() == Qt::Key_J && !event->isAutoRepeat()&&currentBulletType == 0 && bulletCounts[0] > 0) {

        m_doro.startStun(TIMETTT);
        QTimer::singleShot(TIMETTT, [this]() {
            m_doro.m_isStunned = false;
        });
    }

    if(event->key() == Qt::Key_E && !event->isAutoRepeat()) {
        currentBulletType = 1 - currentBulletType;
    }


    if(event->key() == Qt::Key_Z && !event->isAutoRepeat()) {
        m_doro.startDash();

    }


    if(event->key() == Qt::Key_J && !event->isAutoRepeat()) {
        if(bulletCounts[currentBulletType] > 0) {
            // 根据方向计算起始位置和速度
            int direction = m_doro.m_facingRight ? 1 : -1;


            QPoint startPos;
            if(currentBulletType == 1) {
                startPos = QPoint(m_doro.m_X, m_doro.m_Y+30);
            }
            else {
                if(direction==1)
                    startPos = QPoint(m_doro.m_X, m_doro.m_Y-190);
                else
                    startPos = QPoint(m_doro.m_X-550, m_doro.m_Y-190);
            }


            Bullet* newBullet = new Bullet(startPos, currentBulletType, this);
            newBullet->SPEED *= direction;  // 设置速度方向
            m_bullets.append(newBullet);
            bulletCounts[currentBulletType]--;
        }
    }


    if(event->key() == Qt::Key_Space) {
        // 当站在敌人身上时禁止跳跃
        if(!m_doro.isOnEnemy()) {
            m_doro.jump();
        }
        m_pressedKeys.remove(Qt::Key_Space);  // 防止持续触发
    }

    if (event->key() == Qt::Key_K) {
        maodie=false;
        m_doro.enterForm3();

    }
    if (event->key() == Qt::Key_L) {
        maodie=true;
        m_doro.exitForm3();
    }


    switch(event->key()) {
    case Qt::Key_A:
        m_doro.moveLeft();
        break;
    case Qt::Key_D:
        m_doro.moveRight();
        break;
    case Qt::Key_Space:
        m_doro.jump();
        break;

    }

    if(!event->isAutoRepeat()) {  // 防止长按重复触发
        m_pressedKeys.insert(event->key());
    }



    // 形态切换（Q键）
    if(event->key() == Qt::Key_Q && !event->isAutoRepeat()) {
        m_doro.toggleForm();
        update();
    }

    // 垂直移动（W/S键）
    if(event->key() == Qt::Key_S) {
        m_doro.verticalMove(1);
    }
    if(event->key() == Qt::Key_W) {
        m_doro.verticalMove(-1);
    }

}

void MainScene::updateBullets() {
    for(auto bullet : m_bullets) {
        bullet->update();
    }
}

void MainScene::keyReleaseEvent(QKeyEvent* event) {
    m_pressedKeys.remove(event->key());

    if(event->key() == Qt::Key_W || event->key() == Qt::Key_S) {
        m_doro.verticalMove(0);
    }
}

void MainScene::updateEnemies() {
    for (auto it = m_enemies.begin(); it != m_enemies.end();) {
        (*it)->update();
        if (!(*it)->isAlive()) {
            if ((*it)->enemyType == Enemy::Type::Boss) {
                m_score += 1000;
            }
            delete *it;
            it = m_enemies.erase(it);
            m_score += 100;
        } else {
            ++it;
        }
    }
}

void MainScene::collisionDetection() {
    auto bulletIt = m_bullets.begin();
    while (bulletIt != m_bullets.end()) {
        bool bulletHit = false;
        Bullet* bullet = *bulletIt;

        auto enemyIt = m_enemies.begin();
        while (enemyIt != m_enemies.end()) {
            Enemy* enemy = *enemyIt;

            if (bullet->rect.intersects(enemy->getRect())) {  // 使用实际矩形检测
                enemy->takeDamage(bullet->getDamage());
                bulletHit = true;  // 仅标记击中，不立即移除子弹

                if (!enemy->isAlive()) {
                    delete enemy;
                    enemyIt = m_enemies.erase(enemyIt);
                    m_score += 100;
                    continue;
                }
            }
            ++enemyIt;
        }

        // 移除立即删除子弹的逻辑
        ++bulletIt;
    }
}

void MainScene::cleanBullets() {
    auto it = m_bullets.begin();
    while(it != m_bullets.end()) {
        if(!(*it)->isActive()) {  // 仅根据激活状态清理
            delete *it;
            it = m_bullets.erase(it);
        } else {
            ++it;
        }
    }
}

void MainScene::checkCharacterCollisions() {
    m_doro.m_isOnEnemy = false; // 每帧重置状态

    QRect doroFeet = m_doro.m_Rect.adjusted(0, m_doro.m_Rect.height() - 15, 0, 0);

    foreach (Enemy* enemy, m_enemies) {
        QRect enemyHead = enemy->getRect().adjusted(0, -20, 0, 5);

        if (doroFeet.intersects(enemyHead)) {
            int overlap = doroFeet.bottom() - enemyHead.top();
            if (overlap >= 1 && overlap <= 25 && m_doro.m_velocityY >= 0) {
                m_doro.m_isOnEnemy = true; // 标记敌人站立状态

                // 禁止重置地面状态
                m_doro.m_Y = enemy->position.y() - m_doro.m_Rect.height() + 3;
                m_doro.m_velocityY = 0;
                m_doro.m_Rect.moveTo(m_doro.m_X, m_doro.m_Y);
            }
            if (m_doro.isShieldActive()) {
                int damage = 1; // 单次碰撞伤害值
                m_doro.takeDamage(damage); // 护盾优先吸收伤害
            } else {
                m_doro.health--; // 直接扣除生命值
            }
            if (m_doro.health <= 0) {
                gameOver("游戏结束！");
            } else {
                update();  // 手动触发界面重绘
            }
        }
    }


}

void MainScene::spawnEnemy() {

    const int MAX_ENEMIES = 10; // 最大同时存在的敌人数量

    if(m_enemies.size() >= MAX_ENEMIES) {
        return; // 敌人数量已达上限
    }
    // 地面敌人生成（原有逻辑）
    if(QRandomGenerator::global()->bounded(100) < 50) { // 概率生成地面敌人
        m_enemies.append(new Enemy(QPoint(QRandomGenerator::global()->bounded(GAME_WIDTH), GAME_HEIGHT - 150)));
    }

    // 新增飞行敌人生成（20%概率）
    if(QRandomGenerator::global()->bounded(100) < 50) {
        m_enemies.append(new Enemy( QPoint(QRandomGenerator::global()->bounded(GAME_WIDTH), QRandomGenerator::global()->bounded(100, GAME_HEIGHT/2)), Enemy::Type::Flying));
    }

    if (m_score >= 1200 && !m_bossSpawned) {
        // 生成在屏幕右侧固定位置
        QPoint bossPos(GAME_WIDTH-300, GROUND_Y-100);
        m_enemies.append(new Enemy(bossPos, Enemy::Type::Boss));
        m_bossSpawned = true;
        qDebug() << "Boss spawned at position:" << bossPos;
        m_bossTextVisible = true;
        m_bossTextTimer.start(4000); // 4秒后隐藏字幕
    }
}

void MainScene::gameOver(const QString &msg) {
    QMessageBox::critical(this, "游戏结束", msg);
    QApplication::quit();
}

void MainScene::gameWin(const QString &msg) {

    QMessageBox::information(this, "胜利", msg);
    QApplication::quit();
}
