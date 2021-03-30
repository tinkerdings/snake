#ifndef RENDER_H
#define RENDER_H

#include "window.h"
#include "stateHandler.h"
#include "game.h"

class Render
{
public:
    Render(const Render&) = delete;

    static Render& getSingleton()
    {
        return s_Render;
    }

    void init();
    void setBG(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void clear();
    void render();
    void show();

    SDL_Renderer *rend;
private:
    Render(){}
    static Render s_Render;

    Window& wnd = Window::getSingleton();
    StateHandler& state = StateHandler::getSingleton();
    Game& game = Game::getSingleton();

    void (Render:: *internalRender)() = NULL; // Func ptr to current state render func.
    void setRenderPtr();
    void renderMenuMain();
    void renderMenuLvl();
    void renderMenuSettings();
    void renderPlay();
    void renderSnakes();
};

#endif // RENDER_H
