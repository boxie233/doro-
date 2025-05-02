#ifndef MAP_H
#define MAP_H
#include<QPixmap>
class Map
{
public:
    Map();

    void mapPosition();



    QPixmap m_map1;
    QPixmap m_map2;
    int m_map1_posX;
    int m_map2_posX;

    int m_scroll_speed;




};

#endif // MAP_H
