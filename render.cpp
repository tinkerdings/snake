#include <iostream>
#include <SDL2/SDL.h>
#include "render.h"

Render Render::s_Render;
void
Render::init()
{
    rend = SDL_CreateRenderer(wnd.getWindow(), -1, SDL_RENDERER_ACCELERATED);
    if(!rend)
    {
        std::cerr << "SDL_CreateRenderer: " << SDL_GetError() << std::endl;
        std::exit(1);
    }
}

void
Render::setBG(unsigned char r, unsigned char g, unsigned char b)
{
    SDL_SetRenderDrawColor(rend, r, g, b, 255);
}

void
Render::clear()
{
    SDL_RenderClear(rend);
}

void
Render::show()
{
    SDL_RenderPresent(rend);
}

void
Render::renderQueue()
{

}
