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
    TTF_Font *fontInput = NULL;
    SDL_Texture *alphabet[26];
    SDL_Texture *bgUIFramed;
    SDL_Texture *bgUI;
    SDL_Texture *logo;
    SDL_Texture *previewFrame;
    std::vector<SDL_Texture *> allTextures;

    Render(const Render&) = delete;
    static Render& getSingleton()
    {
        return s_Render;
    }
    void init();
    void freeAllTextures();
    void setClear(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void clear();
    void renderBG(SDL_Texture *tex);
    void renderLogo();
    void renderSnakes();
    void renderPickups();
    void show();
    void renderText(Text *tex, int x, int y, int w, int h);
    void initText(Text *tex, const char* txt, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void renderButtons();
    void renderMap();
    void renderTilePlacementBox();
    void renderTilePreview();
    void renderScores();
    void renderBorders();
    void renderLetter(char letter, int x, int y, int w, int h);
    void renderMapNameInput();
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

    void initAlphabet();
};

#endif // RENDER_H
