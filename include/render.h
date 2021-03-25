#ifndef RENDER_H
#define RENDER_H
#include "window.h"

class Render
{
public:
    Render(const Render&) = delete;

    static Render& getSingleton()
    {
        return s_Render;
    }

    void init();
    void setBG(unsigned char r, unsigned char g, unsigned char b);
    void clear();
    void show();

private:
    Render(){}
    Window& wnd = Window::getSingleton();
    SDL_Renderer *rend;
    static Render s_Render;
};

#endif // RENDER_H
