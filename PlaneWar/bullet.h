// bullet.h
#ifndef BULLET_H
#define BULLET_H
#include <QElapsedTimer>
#include <QObject>
#include <QPixmap>
#include <QRect>

class Bullet : public QObject
{
    Q_OBJECT
public:
   explicit Bullet(QPoint startPos, int type, QObject *parent = nullptr);
    void update();
    bool isOutOfScreen() const;
    QPixmap sprite;
    QRect rect;


    int SPEED = 5;




private:
   bool m_isDelayed;       // 延迟状态
  QElapsedTimer m_delayTimer;     // 延迟计时器
};

#endif // BULLET_H
