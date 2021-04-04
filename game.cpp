#include "game.h"
#include <iostream>
#include <functional>
#include "menu.h"
#include "render.h"

Game Game::s_Game;
void
Game::init()
{
    Menu& menu = Menu::getSingleton();
    Render& rend = Render::getSingleton();
    int ww, wh;
    wnd.getSize(ww, wh);
    snakes.clear();
    Snake player1 = {true, ww/2, wh/2};
    snakes.push_back(player1);
    map.loadMap();
    started = true;
}

void
Game::restart()
{
    if (snakes.size())
        snakes.clear();
    init();
}

void
Game::update()
{
    currentTime = SDL_GetTicks();
    for(auto iter = snakes.begin(); iter != snakes.end(); iter++)
    {
        if(currentTime > lastTime + iter->stepDelay)
        {
            iter->update();
            lastTime = currentTime;
        }
    }
}
