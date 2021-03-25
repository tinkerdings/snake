#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <SDL2/SDL.h>
#include "window.h"
#include "render.h"
#include "stateHandler.h"

class InputHandler
{
public:
    InputHandler(const InputHandler&) = delete;

    static InputHandler& getSingleton()
    {
        return s_InputHandler;
    }

    void handle();

private:
    InputHandler(){}
    static InputHandler s_InputHandler;

    Window& wnd = Window::getSingleton();
    Render& rend = Render::getSingleton();
    StateHandler& state = StateHandler::getSingleton();
    SDL_Event e;
    void (InputHandler:: *internalHandle)() = NULL; // Function ptr to current state input handle function.
    void setHandlePtr();
    void handleMenu();
    void handlePlay();
};

#endif // INPUTHANDLER_H
