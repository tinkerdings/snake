#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <vector>
#include <SDL.h>
#include "window.h"

class InputHandler
{
public:
    InputHandler(const InputHandler&) = delete;

    static InputHandler& getSingleton()
    {
        return s_InputHandler;
    }

    void checkQuit();
    void inputPlay();
    void pollInput();
private:
    InputHandler(){}
    static InputHandler s_InputHandler;
    Window& wnd = Window::getSingleton();
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    SDL_Event e;

    // Key input
    bool keyPress(int sdlKeycode);
    bool keyHold(int iKeyIndex);
    bool keyRelease(int sdlKeycode);
};

#endif // INPUTHANDLER_H
