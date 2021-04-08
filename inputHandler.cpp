#include "inputHandler.h"
#include <iostream>
#include <cstdio>
#include "game.h"
#include "menu.h"

InputHandler InputHandler::s_InputHandler;

void
InputHandler::commonInput()
{
    if(e.type == SDL_QUIT)
        wnd.quit();
    if(keyRelease(SDLK_ESCAPE))
        wnd.quit();
    updateMouse();
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
    Window& wnd = Window::getSingleton();
    int ww, wh;
    wnd.getSize(ww, wh);

    if(mouseRelease(SDL_BUTTON_LEFT))
    {
        menu.checkButtons();
    }
    if((e.button.x >= 0) && (e.button.x <= ww) && (e.button.y >= 0) && (e.button.y <= wh))
    {
        if(mouseDown(SDL_BUTTON_LEFT))
        {
            map.setTile(e.button.x, e.button.y, TWALL);
        }
        if(mouseDown(SDL_BUTTON_RIGHT))
        {
            map.setTile(e.button.x, e.button.y, TEMPTY);
        }
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

void
InputHandler::updateMouse()
{
    switch(e.type)
    {
    case(SDL_MOUSEBUTTONDOWN):
    {
        switch(e.button.button)
        {
        case(SDL_BUTTON_LEFT):
        {
            mouseButton[0].prevDown = mouseButton[0].curDown;
            mouseButton[0].curDown = true;
            break;
        }
        case(SDL_BUTTON_RIGHT):
        {
            mouseButton[1].prevDown = mouseButton[1].curDown;
            mouseButton[1].curDown = true;
            break;
        }
        }
        break;
    }
    case(SDL_MOUSEBUTTONUP):
    {
        switch(e.button.button)
        {
        case(SDL_BUTTON_LEFT):
        {
            mouseButton[0].prevDown = mouseButton[0].curDown;
            mouseButton[0].curDown = false;
            break;
        }
        case(SDL_BUTTON_RIGHT):
        {
            mouseButton[1].prevDown = mouseButton[1].curDown;
            mouseButton[1].curDown = false;
            break;
        }
        }
        break;
    }
    }
}
bool
InputHandler::mousePress(int sdlMousecode)
{
    switch(sdlMousecode)
    {
    case(SDL_BUTTON_LEFT):
    {
        if(!mouseButton[0].prevDown && mouseButton[0].curDown)
        {
            return true;
        }
        break;
    }
    case(SDL_BUTTON_RIGHT):
    {
        if(!mouseButton[1].prevDown && mouseButton[1].curDown)
        {
            return true;
        }
        break;
    }
    }
    return false;
}

bool
InputHandler::mouseDown(int sdlMousecode)
{
    switch(sdlMousecode)
    {
    case(SDL_BUTTON_LEFT):
    {
        if(mouseButton[0].curDown)
        {
            return true;
        }
        break;
    }
    case(SDL_BUTTON_RIGHT):
    {
        if(mouseButton[1].curDown)
        {
            return true;
        }
        break;
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
        if(mouseButton[0].prevDown && !mouseButton[0].curDown)
        {
            return true;
        }
        break;
    }
    case(SDL_BUTTON_RIGHT):
    {
        if(mouseButton[1].prevDown && !mouseButton[1].curDown)
        {
            return true;
        }
        break;
    }
    }
    return false;
}
