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

    SDL_Event e;
    void checkQuit();
    void inputPlay();
    void inputCreate();
    void inputMenu();
    void pollInput();
    // Key input
    bool keyPress(int sdlKeycode);
    bool keyHold(int iKeyIndex);
    bool keyRelease(int sdlKeycode);
    bool mousePress(int sdlMousecode);
    bool mouseRelease(int sdlMousecode);
private:
    InputHandler(){}
    static InputHandler s_InputHandler;
    Window& wnd = Window::getSingleton();
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    bool mouseButton[2] = {0};

};

#endif // INPUTHANDLER_H
