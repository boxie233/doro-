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
#define MAP_PATH2 ":/res/background2.png"
#define MAP_SCROLL_SPEED 1
#define GROUND_Y (GAME_HEIGHT - 150)



//doro
#define DORO_PATH ":/res/doro01..png"
#define DORO_PATH2 ":/res/doro0..png"
#define MOVE_SPEED 1
#define TIMETTT 3000//大招

const char DORO_FORM2_RIGHT[] = ":/res/talesi01.png";
const char DORO_FORM2_LEFT[] = ":/res/talesi0.png";


#define DORO_PATH00 ":/res/youxia1.png"
#define DORO_PATH01 ":/res/youxia.png"
#define SCORE 3000




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
#define FLYING_ENEMY_PATH ":/res/enemy2.png"
#define FLYING_ENEMY_PATH2 ":/res/enemy20.png"
#define FLYING_ENEMY_SPEED 2
#define FLYING_ENEMY_HEALTH 40

#define BOSS_PATH_LEFT ":/res/enemy40.png"
#define BOSS_PATH_RIGHT ":/res/enemy4.png"
#define BOSS_SPEED 1
#define BOSS_HEALTH 50000
#define BOSS_JUMP_INTERVAL 5000 // 跳跃间隔
#define BOSS_JUMP_FORCE -18.0f  // 基础跳跃力度
const int ENEMY_SPAWN_INTERVAL = 5000;  //刷新速率
#define SHIELD_RIGHT_SPRITE_PATH ":/res/doro50.png"
#define SHIELD_LEFT_SPRITE_PATH  ":/res/doro5.png"

#define SHIELD_PATH ":/res/shield.png"

//音乐
#define MUSIC_PATH "qrc:/res/music1.mp3"


#define BOSSGRAVITY 0.8f          // 统一重力参数
#define BOSS_GROUND_LEVEL (GAME_HEIGHT - 150) // 根据BOSS精灵高度调整


const float ON_ENEMY_JUMP_MULTIPLIER = 0.4f;
const float DASH_SPEED = 12.0f;    // 突刺速度
const int DASH_DISTANCE = 400;     // 突刺距离
const int DASH_COOLDOWN = 0;    // 冷却时间2秒
const QColor DASH_COLOR = QColor(128, 0, 128, 128); // 残影颜色



#endif // CONFIG_H
