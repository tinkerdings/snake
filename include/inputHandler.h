#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <SDL2/SDL.h>
#include "window.h"

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
    SDL_Event e;
};

#endif // INPUTHANDLER_H
