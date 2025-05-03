#ifndef CONFIG_H
#define CONFIG_H
#include <QCOLOR>

#define GAME_WIDTH 1279
#define GAME_HEIGHT 700
#define GAME_TITLE "欧润橘大作战 v1.0"
#define GAME_ICON ":/res/doro1.png"
#define GAME_RATE 10



//地图
#define MAP_PATH ":/res/background1.png"
#define MAP_SCROLL_SPEED 0



//doro
#define DORO_PATH ":/res/doro01..png"
#define DORO_PATH2 ":/res/doro0..png"
#define MOVE_SPEED 1
#define TIMETTT 3000



//子弹
#define BULLET_PATH ":/res/orange.1.png"
#define BULLET_PATH2 ":/res/orange.2.png"
#define ORANGENUMBER1 1314
#define ORANGENUMBER2 520

//敌人
#define ENEMY_PATH_RIGHT ":/res/enemy10.png"
#define ENEMY_PATH_LEFT ":/res/enemy1.png"
#define ENEMY_SPEED 1
#define ENEMY_HEALTH 60
#define DOGDORO_PATH ":/res/doroene.png"


const float DASH_SPEED = 12.0f;    // 突刺速度
const int DASH_DISTANCE = 400;     // 突刺距离
const int DASH_COOLDOWN = 0;    // 冷却时间2秒
const QColor DASH_COLOR = QColor(128, 0, 128, 128); // 残影颜色

#endif // CONFIG_H
