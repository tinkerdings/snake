#ifndef MAP_H
#define MAP_H

#define GRIDSIZE 20
#define MAPW 40
#define MAPH 30

#include <vector>
#include "pickup.h"
#include "window.h"

enum TileType
{
    TEMPTY, TPICKUP, TWALL, TSNAKE, TP1START, TP2START
};

class Map
{
public:
    std::vector<Pickup> pickups;
    int gridSize = GRIDSIZE;
    TileType map[MAPW][MAPH] = {TEMPTY};
    SDL_Texture *bg;
    SDL_Texture *texWall, *texWallCorner;
    SDL_Texture *texP1Start, *texP2Start;
    int mapW = MAPW * GRIDSIZE;
    int mapH = MAPH * GRIDSIZE;
    int mapX = 80;
    int mapY = 100;
    int mapNW = MAPW;
    int mapNH = MAPH;
    int mapBottom = 20;

    int editorTileIndex = 0;
    std::vector<TileType> editorTiles = {
        TWALL, TP1START, TP2START
    };
    TileType editorActiveTile = editorTiles[editorTileIndex];

    Map(const Map&) = delete;
    static Map& getSingleton()
    {
        return s_Map;
    }

    void loadMap();
    void setTile(int xPos, int yPos, TileType val);
    void resetMap();
    void saveMap();
    void nextEditorTile();
    void prevEditorTile();
    TileType getTile(int xPos, int yPos);

private:
    static Map s_Map;
    Window& wnd = Window::getSingleton();

    Map(){editorActiveTile = TWALL;}
};

#endif // MAP_H
