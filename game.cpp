#include "game.h"
#include <iostream>


Game Game::s_Game;
void
Game::init()
{
    int ww, wh;
    wnd.getSize(ww, wh);
    Snake player1 = {true, ww/2, wh/2};
    snakes.push_back(player1);
}

void
Game::update()
{
    currentTime = SDL_GetTicks();
    if(currentTime > lastTime + 100)
    {
        for(auto iter = snakes.begin(); iter != snakes.end(); iter++)
        {
            iter->update();
        }
        lastTime = currentTime;
    }
}
