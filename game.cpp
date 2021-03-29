#include "game.h"

Game Game::s_Game;
Game::Game()
{
    int ww, wh;
    wnd.getDimentions(ww, wh);
    Snake player1 = {true, 20, 20, ww/2, wh/2};
    snakes.push_back(player1);
}
