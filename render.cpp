#include <iostream>
#include <SDL2/SDL.h>
#include "render.h"

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
Render::setBG(unsigned char r, unsigned char g, unsigned char b)
{
    SDL_SetRenderDrawColor(rend, r, g, b, 255);
}

void
Render::clear()
{
    SDL_RenderClear(rend);
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
