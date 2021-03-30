#include <iostream>
#include "window.h"

Window Window::s_wnd;
void
Window::init(const char* title, int w, int h)
{
    winW = w;
    winH = h;
    wnd = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            winW, winH, SDL_WINDOW_OPENGL);
    if(!wnd)
    {
        std::cerr << "SDL_CreateWindow: " << SDL_GetError() << std::endl;
        std::exit(1);
    }

    quitting = false;
}

void
Window::getSize(int &w, int &h)
{
    w = winW;
    h = winH;
}
