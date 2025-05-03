#include "freeflyenemy.h"
#include <QTimer>
#include <cmath>
#include "config.h"

FreeFlyEnemy::FreeFlyEnemy(QPoint startPos, QObject *parent)
    : Enemy(startPos, parent) {
    m_pixmap = QPixmap(ENEMY_PATH).scaled(40, 40);
}

void FreeFlyEnemy::update() {
    // 水平移动
    m_position.rx() -= m_speed;

    // 正弦垂直运动
    m_angle += m_frequency;
    m_position.ry() += static_cast<int>(m_amplitude * sin(m_angle));

    // 自动销毁（飞出屏幕）
    if (m_position.x() < -50) {
        m_destroyed = true;
    }
}
