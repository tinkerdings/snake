#ifndef PICKUP_H
#define PICKUP_H

#include <SDL.h>

class Pickup
{
public:
    SDL_Rect rect;
    SDL_Color color;

    Pickup(int x, int y, int w, int h, int r, int g, int b, int a);
    void setPosition(int x, int y);
    void setColor(int r, int g, int b, int a);
};

#endif // PICKUP_H
