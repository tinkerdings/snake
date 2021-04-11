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
    Snake player1;
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
    for(auto snake = snakes.begin(); snake != snakes.end(); snake++)
    {
        if(currentTime > lastTime + snake->stepDelay)
        {
            snake->update();
            lastTime = currentTime;
        }
//         snake->updateNeighbors();
//         snake->updateTextures();
    }
}
