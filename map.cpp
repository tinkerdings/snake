#include "map.h"

Map Map::s_Map;
void
Map::loadMap()
{
    int ww, wh;
    wnd.getSize(ww, wh);
    pickups.clear();
    Pickup pickup(ww/2, wh/2 - (3*gridSize), gridSize, gridSize, 255, 32, 32, 255);
    pickups.push_back(pickup);
}
