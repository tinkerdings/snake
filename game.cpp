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
    for(auto snake : snakes)
    {
        snake.update();
    }
}
