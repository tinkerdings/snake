#include <iostream>
#include "snake.h"

Snake::Snake(bool isPlayer, int x, int y)
{
    SnakeSegment head = {.rect={x - (segmentSize/2), y-(segmentSize/2), segmentSize, segmentSize}};
    segments.push_back(head);
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

