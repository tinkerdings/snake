#include "inputHandler.h"

InputHandler InputHandler::s_InputHandler;
void
InputHandler::setHandlePtr()
{
    STATE currentState = state.current();
    switch(currentState)
    {
    case(STATE_MENU_MAIN):
    {
        internalHandle = &handleMenu;
    }
    }
}
void
InputHandler::handle()
{
    SDL_PollEvent(&e);
    internalHandle();
}
