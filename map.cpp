#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <string.h>
#endif

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "map.h"
#include "inputHandler.h"
#include "menu.h"
#include "render.h"

Map Map::s_Map;

void
Map::getMapFileNames(const char *path)
{
#ifdef _WIN32 // WINDOWS SPECIFIC CODE
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = FindFirstFile(path, &FindFileData);
    if(hFind == INVALID_HANDLE_VALUE)
    {
        return;
    } 
    else do
    {
        if((strcmp(pdir->d_name, ".") != 0) && (strcmp(pdir->d_name, "..") != 0))
        {
            mapFileNames.push_back(pdir->d_name);
        }
    } while (FindNextFile(hFind, &FindFileData));
    FindClose(hFind);
#else         // UNIX BASED SPECIFIC CODE
    DIR *dir;
    dirent* pdir;

    dir = opendir(path);

    while(pdir = readdir(dir))
    {
        if((strcmp(pdir->d_name, ".") != 0) && (strcmp(pdir->d_name, "..") != 0))
        {
            mapFileNames.push_back(pdir->d_name);
        }
    }
#endif
}

void
Map::readMap(const char* mapName, char buffer[MAPH*(MAPW+1)])
{
    std::stringstream ss;
    ss << "./maps/" << std::string(mapName);
    std::string filename = ss.str();
    std::ifstream file(filename);
    if(!file)
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    int mapSize = MAPH*MAPW;
    while(!file.eof())
    {
        file.read(buffer, MAPH*(MAPW+1));
    }
    file.close();
}

void
Map::loadMap(const char* mapName)
{
    Render& rend = Render::getSingleton();
    char mapData[MAPH*(MAPW+1)] = {0};
    newMap = false;
    clearMap();
    readMap(mapName, mapData);
    for(int j = 0; j < MAPH; j++)
    {
        for(int i = 0; i < MAPW; i++)
        {
            switch(mapData[(j*(MAPW+1))+i])
            {
            case('0'):
            {
                setTile(j, i, TEMPTY);
                
                break;
            }
            case('#'):
            {
                setTile(j, i, TWALL);
                break;
            }
            case('H'):
            {
                setTile(j, i, TP1HEAD);
                P1HeadStartX = i;
                P1HeadStartY = j;
                
                break;
            }
            case('T'):
            {
                setTile(j, i, TP1TAIL);
                P1TailStartX = i;
                P1TailStartY = j;

                break;
            }
            case('h'):
            {
                setTile(j, i, TP2HEAD);
                P2HeadStartX = i;
                P2HeadStartY = j;
                break;
            }
            case('t'):
            {
                setTile(j, i, TP2TAIL);
                P2TailStartX = i;
                P2TailStartY = j;
                break;
            }
            }
        }
    }
//     pickups.clear();
//     Pickup pickup(mapX + (mapW/2), mapY + (mapH/2) - (3*gridSize), gridSize, gridSize);
//     pickups.push_back(pickup);
}

void
Map::setTile(int row, int column, TileType val)
{
    InputHandler& input = InputHandler::getSingleton();
    switch(val)
    {
    case(TP1HEAD):
    case(TP1TAIL):
    case(TP2HEAD):
    case(TP2TAIL):
    case(TSNAKE):
    case(TWALL):
    {
        map[row][column] = val;
        break;
    }
    case(TEMPTY):
    {
        if((column == P1HeadStartX) && (row == P1HeadStartY) ||
            ((column == P1TailStartX) && (row == P1TailStartY)))
        {
            map[P1HeadStartX][P1HeadStartY] = TEMPTY;
            map[P1TailStartX][P1TailStartY] = TEMPTY;
            P1HeadStartX = -1;
            P1HeadStartY = -1;
            P1TailStartX = -1;
            P1TailStartY = -1;
        }
        else if((column == P2HeadStartX) && (row == P2HeadStartY) ||
            ((column == P2TailStartX) && (row == P2TailStartY)))
        {
            map[P2HeadStartX][P2HeadStartY] = TEMPTY;
            map[P2TailStartX][P2TailStartY] = TEMPTY;
            P2HeadStartX = -1;
            P2HeadStartY = -1;
            P2TailStartX = -1;
            P2TailStartY = -1;
        }
        else
        {
            map[row][column] = val;
        }

        break;
    }
    case(TP1START):
    {
        if(editorValidPlacement)
        {
            if(P1HeadStartX != -1) 
            {
                map[P1HeadStartY][P1HeadStartX] = TEMPTY;
                map[P1TailStartY][P1TailStartX] = TEMPTY;
            }
            switch(editorRotation)
            {
            case(0):
            {
                map[row][column] = TP1HEAD;
                map[row + 1][column] = TP1TAIL;
                P1TailStartX = column;
                P1TailStartY = row + 1;
                break;
            }
            case(90):
            {
                map[row][column] = TP1HEAD;
                map[row][column - 1] = TP1TAIL;
                P1TailStartX = column - 1;
                P1TailStartY = row;
                break;
            }
            case(180):
            {
                map[row][column] = TP1HEAD;
                map[row - 1][column] = TP1TAIL;
                P1TailStartX = column;
                P1TailStartY = row - 1;
                break;
            }
            case(270):
            {
                map[row][column] = TP1HEAD;
                map[row][column + 1] = TP1TAIL;
                P1TailStartX = column + 1;
                P1TailStartY = row;
                break;
            }
            }

            P1HeadStartX = column;
            P1HeadStartY = row;
        }

        break;
    }
    case(TP2START):
    {
        if(editorValidPlacement)
        {
            if(P2HeadStartX != -1) 
            {
                map[P2HeadStartY][P2HeadStartX] = TEMPTY;
                map[P2TailStartY][P2TailStartX] = TEMPTY;
            }
            switch(editorRotation)
            {
            case(0):
            {
                map[row][column] = TP2HEAD;
                map[row + 1][column] = TP2TAIL;
                P2TailStartX = column;
                P2TailStartY = row + 1;
                break;
            }
            case(90):
            {
                map[row][column] = TP2HEAD;
                map[row][column - 1] = TP2TAIL;
                P2TailStartX = column - 1;
                P2TailStartY = row;
                break;
            }
            case(180):
            {
                map[row][column] = TP2HEAD;
                map[row - 1][column] = TP2TAIL;
                P2TailStartX = column;
                P2TailStartY = row - 1;
                break;
            }
            case(270):
            {
                map[row][column] = TP2HEAD;
                map[row][column + 1] = TP2TAIL;
                P2TailStartX = column + 1;
                P2TailStartY = row;
                break;
            }
            }

            P2HeadStartX = column;
            P2HeadStartY = row;
        }

        break;
    }
    }
}



TileType
Map::getTile(int row, int column)
{
    return map[row][column];
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
Map::clearMap()
{
    for(int j = 0; j < MAPH; j++)
    {
        for(int i = 0; i < MAPW; i++)
        {
            map[j][i] = TEMPTY;
        }
    }
}

void
Map::inputMapName()
{
    Menu& menu = Menu::getSingleton();
    InputHandler& input = InputHandler::getSingleton();
    int w = 400;
    int h = 100;

    menu.setButtonColorTxt(0, 255, 64, 255);
    menu.createButton(
        NULL, BRELEASE,
        "SAVE AS:",
        mapX + (mapW/2) - (w/2), mapY + (mapH/2) - (h/2), w, h,
        5, BTINPUT);

    savingMap = true;
}

void
Map::saveMap(std::string mapName)
{
    std::stringstream ss;
    ss << "./maps/" << mapName;
    std::string filename = ss.str();

    std::ofstream file(filename);
    if(!file)
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::stringstream saveStream;
    for(int j = 0; j < MAPH; j++)
    {
        for(int i = 0; i < MAPW; i++)
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
