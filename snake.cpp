#include <iostream>
#include <iterator>
#include "snake.h"

Snake::Snake(bool isPlayer, int x, int y)
{
    startX = x;
    startY = y;
    addSegment();
    addMultipleSegments(10);
    dirX = DIR_NONE;
    dirY = DIR_NONE;
}

void
Snake::addSegment()
{
    int nSegments = segments.size();
    if(!nSegments)
    {
        SnakeSegment head = {.rect={startX, startY, segmentSize, segmentSize}};
        segments.push_back(head);
        return;
    }

    auto last = segments.end() - 1;
    SnakeSegment segment = {.rect={last->rect.x, last->rect.y+segmentSize, segmentSize, segmentSize}};
    segments.push_back(segment);
}

void
Snake::addMultipleSegments(int amount)
{
    for(int i = 0; i < amount; i++)
        addSegment();
}

void
Snake::setColor(Uint8 r,Uint8 g, Uint8 b, Uint8 a)
{
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
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
        segments[0].rect.x += dirX * segmentSize;
        segments[0].rect.y += dirY * segmentSize;
    }
}
