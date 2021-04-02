#include "inputHandler.h"
#include <iostream>
#include <cstdio>
#include "game.h"

InputHandler InputHandler::s_InputHandler;

void
InputHandler::checkQuit()
{
    SDL_PollEvent(&e);
    if(e.type == SDL_QUIT)
        wnd.quit();
}
void
InputHandler::inputPlay()
{
    Game& game = Game::getSingleton();
    SDL_PollEvent(&e);
    if(keyRelease(SDLK_ESCAPE))
        wnd.quit();

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

bool InputHandler::keyRelease(int sdlKeycode)
{
    return (e.type == SDL_KEYUP) && (sdlKeycode == e.key.keysym.sym);
}
