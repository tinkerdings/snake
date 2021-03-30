#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <vector>
#include <SDL2/SDL.h>
#include "window.h"
#include "game.h"

class InputHandler
{
public:
    InputHandler(const InputHandler&) = delete;

    static InputHandler& getSingleton()
    {
        return s_InputHandler;
    }

    void inputPlay();
private:
    InputHandler(){}
    static InputHandler s_InputHandler;
    Window& wnd = Window::getSingleton();
    Game& game = Game::getSingleton();
    SDL_Event e;
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);

    // Key input
    bool keyPress(int sdlKeycode);
    bool keyHold(int iKeyIndex);
    bool keyRelease(int sdlKeycode);
};

#endif // INPUTHANDLER_H
