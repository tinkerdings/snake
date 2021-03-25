#include <iostream>
#include "window.h"

Window Window::s_wnd;
void
Window::init(const char* title, int w, int h)
{
    wnd = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            w, h, SDL_WINDOW_OPENGL);
    if(!wnd)
    {
        std::cerr << "SDL_CreateWindow: " << SDL_GetError() << std::endl;
        std::exit(1);
    }

    quitting = false;
}
