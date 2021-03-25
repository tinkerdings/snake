#include "inputHandler.h"

InputHandler InputHandler::s_InputHandler;
void InputHandler::handle()
{
    SDL_PollEvent(&e);
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
        }
        break;
    }
    }
}
