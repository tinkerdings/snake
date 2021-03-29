#include <iostream>
#include <SDL2/SDL.h>
#include "render.h"
#include "snake.h"

Render Render::s_Render;

void
Render::setRenderPtr()
{
    STATE currentState = state.current();
    if(currentState & (STATE_MENU_MAIN))
        internalRender = &Render::renderMenuMain;
    if(currentState & STATE_MENU_LVL)
        internalRender = &Render::renderMenuLvl;
    if(currentState & STATE_MENU_SETTINGS)
        internalRender = &Render::renderMenuSettings;
    if(currentState & STATE_GAME_PLAY)
        internalRender = &Render::renderPlay;
}

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

void Render::renderSnakes()
{
    for(auto snake : game.snakes)
    {
        setBG(snake.color.r, snake.color.g, snake.color.b, snake.color.a);
        for(auto segment : snake.segments)
        {
            SDL_RenderFillRect(rend, &segment.rect);
        }
    }
}

void Render::render()
{
    setRenderPtr();
    if(internalRender == NULL)
        internalRender = &Render::renderMenuMain;

    (this->*internalRender)();
}

void Render::renderMenuMain()
{

}

void Render::renderMenuLvl()
{
    
}

void Render::renderMenuSettings()
{
    
}

void Render::renderPlay()
{

}

void
Render::show()
{
    SDL_RenderPresent(rend);
}
