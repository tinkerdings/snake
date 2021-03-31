#ifndef MAP_H
#define MAP_H

#include <vector>
#include "pickup.h"
#include "window.h"

class Map
{
public:
    std::vector<Pickup> pickups;
    int gridSize = 20;

    Map(const Map&) = delete;
    static Map& getSingleton()
    {
        return s_Map;
    }

    void loadMap();

private:
    static Map s_Map;
    Window& wnd = Window::getSingleton();

    Map(){}
};

#endif // MAP_H
