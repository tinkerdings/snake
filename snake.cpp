#include <iostream>
#include <iterator>
#include "snake.h"
#include "util.h"

Snake::Snake(bool isPlayer, int x, int y)
{
    startX = x;
    startY = y;
    addMultipleSegments(2);
    dirX = DIR_NONE;
    dirY = DIR_NONE;
}

void
Snake::addSegment()
{
    int nSegments = segments.size();
    if(!nSegments)
    {
        SnakeSegment head = {
            .rect={startX, startY, map.gridSize, map.gridSize},
            .color={32, (unsigned char)iRandRange(128, 255), 32}
        };
        segments.push_back(head);
        return;
    }

    auto last = segments.end() - 1;
    SnakeSegment segment = {
        .rect={last->rect.x, last->rect.y+map.gridSize, map.gridSize, map.gridSize},
        .color={32, (unsigned char)iRandRange(128, 255), 32}
    };
    segments.push_back(segment);
}

void
Snake::addMultipleSegments(int amount)
{
    for(int i = 0; i < amount; i++)
        addSegment();
}

void
Snake::setDirection(Dir dir)
{
    switch(dir)
    {
        case(DIR_NONE):
        {
            dirX = 0;
            dirY = 0;
            break;
        }
        case(DIR_UP):
        {
            dirX = 0;
            dirY = -1;
            break;
        }
        case(DIR_DOWN):
        {
            dirX = 0;
            dirY = 1;
            break;
        }
        case(DIR_LEFT):
        {
            dirX = -1;
            dirY = 0;
            break;
        }
        case(DIR_RIGHT):
        {
            dirX = 1;
            dirY = 0;
            break;
        }
    }
}

void
Snake::update()
{
    if(dirX + dirY)
    {
        for(auto iter = segments.end() - 1; iter != segments.begin(); iter--)
        {
            auto prev = iter-1;
            iter->rect = prev->rect;
        }
        segments[0].rect.x += dirX * map.gridSize;
        segments[0].rect.y += dirY * map.gridSize;
    }
//     checkCollision();
    checkPickup();
}

void
Snake::checkPickup()
{
    int ww, wh;
    wnd.getSize(ww, wh);
    for(auto pickup = map.pickups.begin(); pickup != map.pickups.end(); pickup++)
    {
        if((pickup->rect.x == segments[0].rect.x) && (pickup->rect.y == segments[0].rect.y))
        {
            int x = iRandRange(0, ww/map.gridSize) * map.gridSize;
            int y = iRandRange(0, wh/map.gridSize) * map.gridSize;
            pickup->setPosition(x, y);
            addSegment();
        }
    }
}

bool
Snake::checkCollision()
{
    return false;
}
