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




    QRect getRect() const;
    QPoint getPosition() const;
    bool isActive() const;


    QPoint m_position;
    QPixmap m_pixmap;
    bool m_active = true;
    int m_speed = 10;
    int SPEED;

   bool m_isDelayed;       // 延迟状态
  QElapsedTimer m_delayTimer;     // 延迟计时器

    bool m_shouldRemove = false;


  int m_damage;

  int getDamage() const { return m_damage; }
};

#endif // BULLET_H
