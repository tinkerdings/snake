#include "game.h"
#include <iostream>
#include <functional>
#include "menu.h"
#include "render.h"
#include "pickup.h"

Game Game::s_Game;
void
Game::init()
{
    Menu& menu = Menu::getSingleton();
    Render& rend = Render::getSingleton();
    Map& map = Map::getSingleton();

    snakes.clear();

    Snake player1;
    snakes.push_back(player1);
    if(players == 2)
    {
        Snake player2;
        snakes.push_back(player2);
    }
    else
    {
        map.setTile(map.P2HeadRow, map.P2HeadColumn, TEMPTY);
        map.setTile(map.P2TailRow, map.P2TailColumn, TEMPTY);
    }
    started = true;
    spawnPickup();
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
    }
}

void
Game::setSnakeDir(int playerNum, Dir dir)
{
    snakes[playerNum].setDir(dir);
}
