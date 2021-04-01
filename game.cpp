#include "game.h"
#include <iostream>

Game Game::s_Game;
void
Game::init()
{
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
