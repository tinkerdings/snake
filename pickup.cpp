#include "pickup.h"
#include "render.h"

Pickup::Pickup(int x, int y, int w, int h)
{
    Render& rend = Render::getSingleton();
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    tex = rend.createTexture("res/20x20-powerup.png");
}

void
Pickup::setPosition(int x, int y)
{
    rect.x = x;
    rect.y = y;
}
