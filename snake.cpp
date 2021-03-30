#include <iostream>
#include <iterator>
#include "snake.h"

Snake::Snake(bool isPlayer, int x, int y)
{
    SnakeSegment head = {.rect={x, y, segmentSize, segmentSize}};
    SnakeSegment body = {.rect={x, y+segmentSize, segmentSize, segmentSize}};
    segments.push_back(head);
    segments.push_back(body);
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
Snake::update()
{
    auto iter = segments.end();
    iter--;
    segments[0].rect.y = 10;
    while(iter != segments.begin())
    {
        auto next = iter-1;
        iter->rect = next->rect;
        iter--;
    }
}
