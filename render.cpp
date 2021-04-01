#include <cstdio>
#include <iostream>
#include <SDL.h>
#include "render.h"
#include "snake.h"

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
Render::setBG(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    SDL_SetRenderDrawColor(rend, r, g, b, a);
}

void
Render::clear()
{
    SDL_RenderClear(rend);
}

void
Render::renderSnakes()
{
    Game& game = Game::getSingleton();
    for(auto snake : game.snakes)
    {
        for(auto segment : snake.segments)
        {
            setBG(segment.color.r, segment.color.g, segment.color.b, segment.color.a);
            SDL_RenderFillRect(rend, &segment.rect);
        }
    }
}

void
Render::renderPickups()
{
    for(auto pickup : map.pickups)
    {
        setBG(pickup.color.r, pickup.color.g, pickup.color.b, pickup.color.a);
        SDL_RenderFillRect(rend, &pickup.rect);
    }
}

void
Render::renderUI()
{

}

void
Render::show()
{
    SDL_RenderPresent(rend);
}
