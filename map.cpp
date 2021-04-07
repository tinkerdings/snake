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

void
Map::setTile(int xPos, int yPos, TileType val)
{
    map[xPos/gridSize][yPos/gridSize] = val;
}

TileType
Map::getTile(int xPos, int yPos)
{
    return map[xPos/gridSize][yPos/gridSize];
}

void
Map::resetMap()
{
    for(int i = 0; i < MAPH; i++)
    {
        for(int j = 0; j < MAPW; j++)
        {
            map[j][i] = TEMPTY;
        }
    }
}
