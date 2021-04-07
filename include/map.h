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
    TEMPTY, TPICKUP, TWALL, TSNAKE
};

class Map
{
public:
    std::vector<Pickup> pickups;
    int gridSize = GRIDSIZE;
    TileType map[MAPW][MAPH] = {TEMPTY};
    SDL_Texture *bg;

    Map(const Map&) = delete;
    static Map& getSingleton()
    {
        return s_Map;
    }

    void loadMap();
    void setTile(int xPos, int yPos, TileType val);
    void resetMap();
    TileType getTile(int xPos, int yPos);

private:
    static Map s_Map;
    Window& wnd = Window::getSingleton();

    Map(){}
};

#endif // MAP_H
