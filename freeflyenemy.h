#pragma once
#include "enemy.h"

class FreeFlyEnemy : public Enemy {
    Q_OBJECT
public:
    FreeFlyEnemy(QPoint startPos, QObject *parent = nullptr);
    void update() override;

private:
    double m_angle = 0;      // 用于计算正弦波
    double m_frequency = 0.1;// 波动频率
    int m_amplitude = 50;    // 波动幅度
};
