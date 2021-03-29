#include "snake.h"

Snake::Snake(bool isPlayer, int w, int h, int x, int y)
{
    SnakeSegment head;
    head.rect.w = w;
    head.rect.h = h;
    head.rect.x = x;
    head.rect.y = y;
    this->color = color;
}

void Snake::setColor(Uint8 r,Uint8 g, Uint8 b, Uint8 a)
{
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
}
