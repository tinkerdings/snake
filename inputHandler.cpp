#include "inputHandler.h"

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

void
InputHandler::handle()
{
    setHandlePtr();
    if(internalHandle == NULL)
        internalHandle = &InputHandler::handleMenu;

    SDL_PollEvent(&e);
    if(e.type == SDL_QUIT)
    {
        wnd.quit();
    }

    (this->*internalHandle)();
}

void
InputHandler::handleMenu()
{
    rend.setBG(128, 32, 32);
    switch(e.type)
    {
    case(SDL_KEYDOWN):
    {
        switch(e.key.keysym.sym)
        {
        case(SDLK_ESCAPE):
        {
            wnd.quit();
            break;
        }
        case(SDLK_s):
        {
            state.init(STATE_GAME_PLAY);
            break;
        }
        }
    }
    }
}

void
InputHandler::handlePlay()
{
    rend.setBG(32, 32, 128);

    switch(e.type)
    {
    case(SDL_KEYDOWN):
    {
        switch(e.key.keysym.sym)
        {
        case(SDLK_ESCAPE):
        {
            wnd.quit();
            break;
        }
        case(SDLK_s):
        {
            state.init(STATE_MENU_MAIN);
            break;
        }
        }
    }
    }
}
