#include "pickup.h"

Pickup::Pickup(int x, int y, int w, int h, int r, int g, int b, int a)
{
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
}

void
Pickup::setPosition(int x, int y)
{
    rect.x = x;
    rect.y = y;
}
