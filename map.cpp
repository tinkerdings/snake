#include <iostream>
#include "map.h"
#include "render.h"

Map Map::s_Map;
void
Map::loadMap()
{
    Render& rend = Render::getSingleton();
    bg = rend.createTexture("res/20x20-bg.png");
    texWall= rend.createTexture("res/20x20-obstacle.png");
    texWallCorner = rend.createTexture("res/20x20-0bstacle-2.png");
    texP1Start = rend.createTexture("res/20x20-head.png");
    texP2Start = rend.createTexture("res/20x20-head-3.png");
    pickups.clear();
    Pickup pickup(mapX + (mapW/2), mapY + (mapH/2) - (3*gridSize), gridSize, gridSize);
    pickups.push_back(pickup);
}

void
Map::setTile(int xPos, int yPos, TileType val)
{
    if(getTile(xPos, yPos) != val)
        map[(xPos - mapX)/gridSize][(yPos - mapY)/gridSize] = val;
}

TileType
Map::getTile(int xPos, int yPos)
{
    return map[(xPos - mapX)/gridSize][(yPos - mapY)/gridSize];
}

void
Map::nextEditorTile()
{
    editorTileIndex++;
    int nTiles = editorTiles.size();
    if(editorTileIndex >= nTiles)
            editorTileIndex = nTiles-1;
    editorActiveTile = editorTiles[editorTileIndex];
}

void
Map::prevEditorTile()
{
    editorTileIndex--;
    if(editorTileIndex < 0)
            editorTileIndex = 0;
    editorActiveTile = editorTiles[editorTileIndex];
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

void
Map::saveMap()
{
    std::cout << "Map saved!" << std::endl;
}
