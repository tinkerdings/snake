#include "map.h"

Map Map::s_Map;
void
Map::loadMap()
{
    int ww, wh;
    wnd.getSize(ww, wh);
    Pickup pickup(ww/2, wh/2 - 100, gridSize, gridSize, 255, 255, 32, 255);
    pickups.push_back(pickup);
}
