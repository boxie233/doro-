#include "map.h"
#include "config.h"

Map::Map() {

    //初始化
    m_map1.load(MAP_PATH);
    m_map2.load(MAP_PATH);

    m_map1_posX=GAME_WIDTH;
    m_map2_posX=0;

    m_scroll_speed=MAP_SCROLL_SPEED;
}


void Map::mapPosition(){

    m_map1_posX -=m_scroll_speed;
    if(m_map1_posX<=0){
        m_map1_posX=GAME_WIDTH;
    }

    m_map2_posX -=m_scroll_speed;
    if(m_map2_posX<=-GAME_WIDTH){
        m_map2_posX=0;
    }

};
