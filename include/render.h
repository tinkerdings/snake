#ifndef RENDER_H
#define RENDER_H

#include "window.h"
#include "map.h"
#include <SDL_ttf.h>

struct Text
{
    SDL_Texture *tex = NULL;
    int w, h;
    SDL_Rect out;
};
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
    void renderText(SDL_Texture *tex, int x, int y, int w, int h);
    void initText(Text *tex, const char* txt, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

private:
    Render(){}
    static Render s_Render;
    TTF_Font *font = NULL;
    Text textP1Name;
    Text textP2Name;
    Text textP1Score;
    Text textP2Score;

    Window& wnd = Window::getSingleton();
    Map& map = Map::getSingleton();
    void renderScores();
};

#endif // RENDER_H
