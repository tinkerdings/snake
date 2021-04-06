#include "map.h"
#include "render.h"

Map Map::s_Map;
void
Map::loadMap()
{
    Render& rend = Render::getSingleton();
    int ww, wh;
    wnd.getSize(ww, wh);
    bg = rend.createTexture("res/20x20-bg.png");
    pickups.clear();
    Pickup pickup(ww/2, wh/2 - (3*gridSize), gridSize, gridSize);
    pickups.push_back(pickup);
}
