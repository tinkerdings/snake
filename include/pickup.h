#ifndef PICKUP_H
#define PICKUP_H

#include <SDL.h>

class Pickup
{
public:
    SDL_Rect rect;
    SDL_Texture *tex;

    Pickup(int x, int y, int w, int h);
    void setPosition(int x, int y);
};

#endif // PICKUP_H
