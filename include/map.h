#ifndef MAP_H
#define MAP_H

#define GRIDSIZE 20
#define MAPW 40
#define MAPH 30

#include <vector>
#include <string>
#include "pickup.h"
#include "window.h"

enum TileType
{
    TEMPTY, TPICKUP, TWALL, TSNAKE, TP1START, TP2START, TP1HEAD, TP1BODY, TP1TAIL, TP2HEAD, TP2BODY, TP2TAIL
};

class Map
{
public:
    int gridSize = GRIDSIZE;
    TileType map[MAPH][MAPW] = {TEMPTY};
    SDL_Texture *bg;
    SDL_Texture *texWall, *texWallCorner;
    SDL_Texture *texP1Head, *texP1Body, *texP1Tail, *texP2Head, *texP2Body, *texP2Tail;
    int mapW = MAPW * GRIDSIZE;
    int mapH = MAPH * GRIDSIZE;
    
    int mapX = 80;
    int mapY = 100;
    int mapNW = MAPW;
    int mapNH = MAPH;
    int mapBottom = 20;
    std::vector<char> editorSaveName;
    std::vector<std::string> mapFileNames;
    bool savingMap = false;
    bool newMap = true;

    int editorTileIndex = 0;
    std::vector<TileType> editorTiles = {
        TWALL, TP1START, TP2START
    };
    TileType editorActiveTile = editorTiles[editorTileIndex];
    int editorRotation = 0;
    bool editorValidPlacement = true;
    int P1HeadRow       = -1;
    int P1HeadColumn    = -1;
    int P1TailRow       = -1;
    int P1TailColumn    = -1;
    int P2HeadRow       = -1;
    int P2HeadColumn    = -1;
    int P2TailRow       = -1;
    int P2TailColumn    = -1;
    std::string currentMapName;

    Map(const Map&) = delete;
    static Map& getSingleton()
    {
        return s_Map;
    }

    void loadMap(const char* mapName);
    void readMap(const char* mapName, char buffer[MAPH*(MAPW+1)]);
    void printMap();
    void getMapFileNames(const char *path);
    void setTile(int xPos, int yPos, TileType val);
    void clearMap();
    void inputMapName();
    void saveMap(std::string mapName);
    void nextEditorTile();
    void prevEditorTile();
    void editorRotate();
    TileType getTile(int xPos, int yPos);

private:
    static Map s_Map;
    Window& wnd = Window::getSingleton();

    Map(){editorActiveTile = TWALL;}
};

#endif // MAP_H
