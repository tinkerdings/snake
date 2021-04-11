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
    TTF_Font *font = NULL;

    Render(const Render&) = delete;
    static Render& getSingleton()
    {
        return s_Render;
    }
    void init();
    void setClear(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void clear();
    void renderBG();
    void renderSnakes();
    void renderPickups();
    void renderUI();
    void show();
    void renderText(Text *tex, int x, int y, int w, int h);
    void initText(Text *tex, const char* txt, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void renderButtons();
    void renderMap();
    SDL_Texture* createTexture(const char* filename);

private:
    Render(){}
    static Render s_Render;
    Text textP1Name;
    Text textP2Name;
    Text textP1Score;
    Text textP2Score;

    Window& wnd = Window::getSingleton();
    Map& map = Map::getSingleton();
    void renderScores();
    void renderBorders();
};

#endif // RENDER_H
