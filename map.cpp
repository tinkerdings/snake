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
Map::loadMap(const char* mapName)
{
    std::cout << "loaded map" << std::endl;
    Render& rend = Render::getSingleton();
    char mapData[MAPH*(MAPW+1)] = {0};
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
                break;
            }
            case('T'):
            {
                setTile(j, i, TP1TAIL);
                break;
            }
            case('h'):
            {
                setTile(j, i, TP2HEAD);
                break;
            }
            case('t'):
            {
                setTile(j, i, TP2TAIL);
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
}

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
Map::setTile(int row, int column, TileType val)
{
    // in GAME

    // can only place one of each player.
    if(val == TEMPTY)
    {
        if((column == editorP1HeadStartX) && (row == editorP1HeadStartY) ||
            ((column == editorP1TailStartX) && (row == editorP1TailStartY)))
        {
            map[editorP1HeadStartX][editorP1HeadStartY] = TEMPTY;
            map[editorP1TailStartX][editorP1TailStartY] = TEMPTY;
        }
        else if((column == editorP2HeadStartX) && (row == editorP2HeadStartY) ||
            ((column == editorP2TailStartX) && (row == editorP2TailStartY)))
        {
            map[editorP2HeadStartX][editorP2HeadStartY] = TEMPTY;
            map[editorP2TailStartX][editorP2TailStartY] = TEMPTY;
        }
        else
        {
            map[row][column] = val;
        }
    }
    if(val == TSNAKE)
    {
        map[row][column] = val;
    }

    // in EDITOR
    if((editorValidPlacement) && (getTile(column, row) != val))
    {
        switch(val)
        {
        case(TWALL):
        {
            map[row][column] = val;
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
                map[row][column] = TP1HEAD;
                map[row + 1][column] = TP1TAIL;
                editorP1TailStartX = column;
                editorP1TailStartY = row + 1;
                break;
            }
            case(90):
            {
                map[row][column] = TP1HEAD;
                map[row][column - 1] = TP1TAIL;
                editorP1TailStartX = column - 1;
                editorP1TailStartY = row;
                break;
            }
            case(180):
            {
                map[row][column] = TP1HEAD;
                map[row - 1][column] = TP1TAIL;
                editorP1TailStartX = column;
                editorP1TailStartY = row - 1;
                break;
            }
            case(270):
            {
                map[row][column] = TP1HEAD;
                map[row][column + 1] = TP1TAIL;
                editorP1TailStartX = column + 1;
                editorP1TailStartY = row;
                break;
            }
            }

            editorP1HeadStartX = column;
            editorP1HeadStartY = row;
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
                map[row][column] = TP2HEAD;
                map[row + 1][column] = TP2TAIL;
                editorP2TailStartX = column;
                editorP2TailStartY = row + 1;
                break;
            }
            case(90):
            {
                map[row][column] = TP2HEAD;
                map[row][column - 1] = TP2TAIL;
                editorP2TailStartX = column - 1;
                editorP2TailStartY = row;
                break;
            }
            case(180):
            {
                map[row][column] = TP2HEAD;
                map[row - 1][column] = TP2TAIL;
                editorP2TailStartX = column;
                editorP2TailStartY = row - 1;
                break;
            }
            case(270):
            {
                map[row][column] = TP2HEAD;
                map[row][column + 1] = TP2TAIL;
                editorP2TailStartX = column + 1;
                editorP2TailStartY = row;
                break;
            }
            }

            editorP2HeadStartX = column;
            editorP2HeadStartY = row;
            break;
        }
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
