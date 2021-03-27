#include "inputHandler.h"
#include <iostream>

InputHandler InputHandler::s_InputHandler;
void
InputHandler::setHandlePtr()
{
    STATE currentState = state.current();
    if(currentState & (STATE_MENU_MAIN | STATE_MENU_LVL | STATE_MENU_SETTINGS))
        internalHandle = &InputHandler::handleMenu;
    if(currentState & STATE_GAME_PLAY)
        internalHandle = &InputHandler::handlePlay;
}

// Calls one of the state input handlers, depending on the function pointer
// internalHandle.
void
InputHandler::handle()
{
    if(SDL_PollEvent(&e))
    {
        switch(e.type)
        {
        case(SDL_QUIT):
        {
            wnd.quit();
            break;
        }
        }
        setHandlePtr();
        if(internalHandle == NULL)
            internalHandle = &InputHandler::handleMenu;

        (this->*internalHandle)();
    }

}

// [ STATE INPUT HANDLERS ]
void
InputHandler::handleMenu()
{
    if(keyRelease(SDLK_ESCAPE))
    {
        wnd.quit();
    }
}

void
InputHandler::handlePlay()
{
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
