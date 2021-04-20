#include <iostream>
#include "pickup.h"
#include "map.h"
#include "util.h"

void
spawnPickup()
{
    Map& map = Map::getSingleton();
    int row, column;
    while(map.getTile((row = iRandRange(0, MAPH-1)), (column = iRandRange(0, MAPW-1))) != TEMPTY)
        ;
    map.setTile(row, column, TPICKUP);
}
