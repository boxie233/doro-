#include "mainscene.h"
#include "config.h"
#include <QIcon>
#include <QPainter>
#include <QKeyEvent>




MainScene::MainScene(QWidget *parent)
    : QWidget(parent)
{

    initScene();//调用初始化场景

    playGame();

    m_bullets.reserve(50);

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



void MainScene::playGame(){

    m_Timer.start();

    connect(&m_Timer,&QTimer::timeout,[=](){

    handleContinuousInput();

    m_doro.updatePhysics(); // 新增物理更新

    updatePosition();

    update();

    updateBullets();    // 更新子弹位置
    cleanBullets();     // 清理溢出子弹

    });
};




void MainScene::handleContinuousInput() {
    if (m_doro.isStunned()) return; // 定身期间禁止移动

    // 原有移动逻辑
    if (m_pressedKeys.contains(Qt::Key_A)) {
        m_doro.moveLeft();
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


    painter.drawPixmap(m_doro.m_X,m_doro.m_Y,m_doro.m_doro);


    for(const auto bullet : m_bullets) {
        painter.drawPixmap(bullet->rect.topLeft(), bullet->sprite);
    }

      painter.setPen(Qt::white);
      painter.setFont(QFont("Arial", 10));
          QString ammoText = QString("欧润橘%1: %2只").arg(currentBulletType + 1).arg(bulletCounts[currentBulletType]);
           painter.drawText(20, 40, ammoText);


      if (m_doro.isStunned()) {
          painter.setPen(Qt::red);
          painter.setFont(QFont("Arial", 50, QFont::Bold));
          painter.drawText(GAME_WIDTH/2-200, GAME_HEIGHT/2,"奥义*欧润橘");
      }

};



void MainScene::keyPressEvent(QKeyEvent* event) {

    if (event->key() == Qt::Key_J && !event->isAutoRepeat()&&currentBulletType == 0 && bulletCounts[0] > 0) {

            m_doro.startStun(1000);
            QTimer::singleShot(1000, [this]() {
                m_doro.m_isStunned = false;
            });

            bulletCounts[currentBulletType]++;
            bulletCounts[currentBulletType]--;
    }





    if(event->key() == Qt::Key_E && !event->isAutoRepeat()) {
               currentBulletType = 1 - currentBulletType;
    }

    if(event->key() == Qt::Key_J && !event->isAutoRepeat()) {
        if(bulletCounts[currentBulletType] > 0&&currentBulletType == 0) {
        QPoint startPos(m_doro.m_X, m_doro.m_Y-190);
         m_bullets.append(new Bullet(startPos, currentBulletType, this));
        bulletCounts[currentBulletType]--;
        }
        if(bulletCounts[currentBulletType] > 0&&currentBulletType == 1) {
            QPoint startPos(m_doro.m_X, m_doro.m_Y+30);
            m_bullets.append(new Bullet(startPos, currentBulletType, this));
            bulletCounts[currentBulletType]--;
        }

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

}





void MainScene::updateBullets() {
    for(auto bullet : m_bullets) {
        bullet->update();
    }
}





void MainScene::cleanBullets() {
    auto it = m_bullets.begin();
    while(it != m_bullets.end()) {
        if((*it)->isOutOfScreen()) {
            delete *it;
            it = m_bullets.erase(it);
        } else {
            ++it;
        }
    }
}


void MainScene::keyReleaseEvent(QKeyEvent* event) {
    m_pressedKeys.remove(event->key());
}
