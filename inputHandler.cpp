#include "inputHandler.h"
#include <iostream>
#include <cstdio>
#include "game.h"
#include "menu.h"

InputHandler InputHandler::s_InputHandler;

void
InputHandler::checkQuit()
{
    if(e.type == SDL_QUIT)
        wnd.quit();
    if(keyRelease(SDLK_ESCAPE))
        wnd.quit();
    mouseRelease(SDL_BUTTON_LEFT);
    mouseRelease(SDL_BUTTON_RIGHT);
}

void
InputHandler::pollInput()
{
    SDL_PollEvent(&e);
}

void
InputHandler::inputPlay()
{
    Game& game = Game::getSingleton();

    if(keyPress(SDLK_UP) || keyPress(SDLK_w))
    {
        if(game.snakes[0].dirAvailable(DIR_UP))
			game.snakes[0].setDirection(DIR_UP);
    }
    if(keyPress(SDLK_DOWN) || keyPress(SDLK_s))
    {
        if(game.snakes[0].dirAvailable(DIR_DOWN))
			game.snakes[0].setDirection(DIR_DOWN);
    }
    if(keyPress(SDLK_LEFT) || keyPress(SDLK_a))
    {
        if(game.snakes[0].dirAvailable(DIR_LEFT))
			game.snakes[0].setDirection(DIR_LEFT);
    }
    if(keyPress(SDLK_RIGHT) || keyPress(SDLK_d))
    {
        if(game.snakes[0].dirAvailable(DIR_RIGHT))
			game.snakes[0].setDirection(DIR_RIGHT);
    }
}

void
InputHandler::inputCreate()
{
    Map& map = Map::getSingleton();
    Menu& menu = Menu::getSingleton();

    if(mousePress(SDL_BUTTON_LEFT))
    {
        if(!menu.checkButtons())
        {
            map.setTile(e.button.x, e.button.y, TWALL);
        }
    }
    if(mousePress(SDL_BUTTON_RIGHT))
    {
        map.setTile(e.button.x, e.button.y, TEMPTY);
    }
}

void
InputHandler::inputMenu()
{
    Menu& menu = Menu::getSingleton();
    if(mousePress(SDL_BUTTON_LEFT))
    {
        menu.checkButtons();
    }
}

// [ INPUT CHECKING METHODS ]
bool
InputHandler::keyPress(int sdlKeycode)
{
    return (e.type == SDL_KEYDOWN) && ((!e.key.repeat) && (sdlKeycode == e.key.keysym.sym));
}

bool
InputHandler::keyHold(int sdlKeycode)
{
    SDL_PumpEvents();
    return keyboardState[SDL_GetScancodeFromKey(sdlKeycode)];
}

bool
InputHandler::keyRelease(int sdlKeycode)
{
    return (e.type == SDL_KEYUP) && (sdlKeycode == e.key.keysym.sym);
}

bool
InputHandler::mousePress(int sdlMousecode)
{
    switch(sdlMousecode)
    {
    case(SDL_BUTTON_LEFT):
    {
        return !mouseButton[0] && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button == sdlMousecode) && (mouseButton[0] = true);
    }
    case(SDL_BUTTON_RIGHT):
    {
        return !mouseButton[1] && (e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button == sdlMousecode) && (mouseButton[1] = true);
    }
    }

    return false;
}
bool
InputHandler::mouseRelease(int sdlMousecode)
{
    switch(sdlMousecode)
    {
    case(SDL_BUTTON_LEFT):
    {
        return mouseButton[0] && (e.type == SDL_MOUSEBUTTONUP) && (e.button.button == sdlMousecode) && (mouseButton[0] = false);
    }
    case(SDL_BUTTON_RIGHT):
    {
        return mouseButton[1] && (e.type == SDL_MOUSEBUTTONUP) && (e.button.button == sdlMousecode) && (mouseButton[1] = false);
    }
    }

    return false;
}
