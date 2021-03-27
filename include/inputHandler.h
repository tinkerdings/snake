#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <vector>
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

    SDL_Event e;
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    Window& wnd = Window::getSingleton();
    Render& rend = Render::getSingleton();
    StateHandler& state = StateHandler::getSingleton();

    void (InputHandler:: *internalHandle)() = NULL; // Function ptr to current state input handle function.
    void setHandlePtr();
    void handleMenu();
    void handlePlay();

    // Key input
    bool keyPress(int sdlKeycode);
    bool keyHold(int iKeyIndex);
    bool keyRelease(int sdlKeycode);
};

#endif // INPUTHANDLER_H
