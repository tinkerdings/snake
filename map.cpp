#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "map.h"
#include "render.h"

#define HERE std::cout << "here!" << std::endl

Map Map::s_Map;
void
Map::loadMap()
{
    Render& rend = Render::getSingleton();
    bg              = rend.createTexture("res/20x20-bg.png");
    texWall         = rend.createTexture("res/20x20-obstacle.png");
    texWallCorner   = rend.createTexture("res/20x20-0bstacle-2.png");
    texP1Head       = rend.createTexture("res/20x20-head.png");
    texP1Tail       = rend.createTexture("res/20x20-tail.png");
    texP2Head       = rend.createTexture("res/20x20-head-3.png");
    texP2Tail       = rend.createTexture("res/20x20-tail-3.png");
    pickups.clear();
    Pickup pickup(mapX + (mapW/2), mapY + (mapH/2) - (3*gridSize), gridSize, gridSize);
    pickups.push_back(pickup);
}

void
Map::setTile(int xPos, int yPos, TileType val)
{
    // in GAME

    // can only place one of each player.
    if(val == TEMPTY)
    {
        if((((xPos - mapX)/gridSize) == editorP1HeadStartX) && (((yPos - mapY)/gridSize) == editorP1HeadStartY) ||
            ((((xPos - mapX)/gridSize) == editorP1TailStartX) && (((yPos - mapY)/gridSize) == editorP1TailStartY)))
        {
            map[editorP1HeadStartX][editorP1HeadStartY] = TEMPTY;
            map[editorP1TailStartX][editorP1TailStartY] = TEMPTY;
        }
        else if((((xPos - mapX)/gridSize) == editorP2HeadStartX) && (((yPos - mapY)/gridSize) == editorP2HeadStartY) ||
            ((((xPos - mapX)/gridSize) == editorP2TailStartX) && (((yPos - mapY)/gridSize) == editorP2TailStartY)))
        {
            map[editorP2HeadStartX][editorP2HeadStartY] = TEMPTY;
            map[editorP2TailStartX][editorP2TailStartY] = TEMPTY;
        }
        else
        {
            map[(xPos - mapX)/gridSize][(yPos - mapY)/gridSize] = val;
        }
    }
    if(val == TSNAKE)
    {
        map[(xPos - mapX)/gridSize][(yPos - mapY)/gridSize] = val;
    }

    // in EDITOR
    if((editorValidPlacement) && (getTile(xPos, yPos) != val))
    {
        switch(val)
        {
        case(TWALL):
        {
            map[(xPos - mapX)/gridSize][(yPos - mapY)/gridSize] = val;
            break;
        }
        case(TP1START):
        {
            if(editorP1HeadStartX != -1) 
            {
                map[editorP1HeadStartX][editorP1HeadStartY] = TEMPTY;
                map[editorP1TailStartX][editorP1TailStartY] = TEMPTY;
            }
            switch(editorRotation)
            {
            case(0):
            {
                map[(xPos - mapX)/gridSize][(yPos - mapY)/gridSize] = TP1HEAD;
                map[(xPos - mapX)/gridSize][((yPos - mapY)/gridSize) + 1] = TP1TAIL;
                editorP1TailStartX = ((xPos - mapX)/gridSize);
                editorP1TailStartY = ((yPos - mapY)/gridSize) + 1;
                break;
            }
            case(90):
            {
                map[(xPos - mapX)/gridSize][(yPos - mapY)/gridSize] = TP1HEAD;
                map[((xPos - mapX)/gridSize) - 1][(yPos - mapY)/gridSize] = TP1TAIL;
                editorP1TailStartX = ((xPos - mapX)/gridSize) - 1;
                editorP1TailStartY = ((yPos - mapY)/gridSize);
                break;
            }
            case(180):
            {
                map[(xPos - mapX)/gridSize][(yPos - mapY)/gridSize] = TP1HEAD;
                map[(xPos - mapX)/gridSize][((yPos - mapY)/gridSize) - 1] = TP1TAIL;
                editorP1TailStartX = ((xPos - mapX)/gridSize);
                editorP1TailStartY = ((yPos - mapY)/gridSize) - 1;
                break;
            }
            case(270):
            {
                map[(xPos - mapX)/gridSize][(yPos - mapY)/gridSize] = TP1HEAD;
                map[((xPos - mapX)/gridSize) + 1][(yPos - mapY)/gridSize] = TP1TAIL;
                editorP1TailStartX = ((xPos - mapX)/gridSize) + 1;
                editorP1TailStartY = ((yPos - mapY)/gridSize);
                break;
            }
            }

            editorP1HeadStartX = ((xPos - mapX)/gridSize);
            editorP1HeadStartY = ((yPos - mapY)/gridSize);
            break;
        }
        case(TP2START):
        {
            if(editorP2HeadStartX != -1) 
            {
                map[editorP2HeadStartX][editorP2HeadStartY] = TEMPTY;
                map[editorP2TailStartX][editorP2TailStartY] = TEMPTY;
            }
            switch(editorRotation)
            {
            case(0):
            {
                map[(xPos - mapX)/gridSize][(yPos - mapY)/gridSize] = TP2HEAD;
                map[(xPos - mapX)/gridSize][((yPos - mapY)/gridSize) + 1] = TP2TAIL;
                editorP2TailStartX = ((xPos - mapX)/gridSize);
                editorP2TailStartY = ((yPos - mapY)/gridSize) + 1;
                break;
            }
            case(90):
            {
                map[(xPos - mapX)/gridSize][(yPos - mapY)/gridSize] = TP2HEAD;
                map[((xPos - mapX)/gridSize) - 1][(yPos - mapY)/gridSize] = TP2TAIL;
                editorP2TailStartX = ((xPos - mapX)/gridSize) - 1;
                editorP2TailStartY = ((yPos - mapY)/gridSize);
                break;
            }
            case(180):
            {
                map[(xPos - mapX)/gridSize][(yPos - mapY)/gridSize] = TP2HEAD;
                map[(xPos - mapX)/gridSize][((yPos - mapY)/gridSize) - 1] = TP2TAIL;
                editorP2TailStartX = ((xPos - mapX)/gridSize);
                editorP2TailStartY = ((yPos - mapY)/gridSize) - 1;
                break;
            }
            case(270):
            {
                map[(xPos - mapX)/gridSize][(yPos - mapY)/gridSize] = TP2HEAD;
                map[((xPos - mapX)/gridSize) + 1][(yPos - mapY)/gridSize] = TP2TAIL;
                editorP2TailStartX = ((xPos - mapX)/gridSize) + 1;
                editorP2TailStartY = ((yPos - mapY)/gridSize);
                break;
            }
            }

            editorP2HeadStartX = ((xPos - mapX)/gridSize);
            editorP2HeadStartY = ((yPos - mapY)/gridSize);
            break;
        }
        }
    }
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
Map::editorRotate()
{
    editorRotation+=90;
    if(editorRotation == 360)
        editorRotation = 0;
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
Map::saveMap(std::string mapName)
{
    std::stringstream ss;
    ss << mapName << ".map";
    std::string filename = ss.str();

    std::ofstream file(filename);
    if(!file)
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::stringstream saveStream;
    for(int i = 0; i < MAPH; i++)
    {
        for(int j = 0; j < MAPW; j++)
        {
            switch(map[j][i])
            {
            case(TEMPTY):
            {
                saveStream << "0";
                break;
            }
            case(TWALL):
            {
                saveStream << "#";
                break;
            }
            case(TP1HEAD):
            {
                saveStream << "H";
                break;
            }
            case(TP1TAIL):
            {
                saveStream << "T";
                break;
            }
            case(TP2HEAD):
            {
                saveStream << "h";
                break;
            }
            case(TP2TAIL):
            {
                saveStream << "t";
                break;
            }
            default:
            {
                saveStream << "?";
                break;
            }
            }
        }
        saveStream << "\n";
    }
    saveStream << "\0";
    
    std::string saveString = saveStream.str();

    file << saveString;
    file.close();
}
