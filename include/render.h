#ifndef RENDER_H
#define RENDER_H

#include "window.h"
#include "map.h"

class Render
{
public:
    SDL_Renderer *rend;

    Render(const Render&) = delete;
    static Render& getSingleton()
    {
        return s_Render;
    }
    void init();
    void setBG(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void clear();
    void renderSnakes();
    void renderPickups();
    void renderUI();
    void show();

private:
    Render(){}
    static Render s_Render;

    Window& wnd = Window::getSingleton();
    Map& map = Map::getSingleton();
};

#endif // RENDER_H
