#include <cstdio>
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "render.h"
#include "game.h"
#include "snake.h"
#include "menu.h"

Render Render::s_Render;

void
Render::init()
{
    rend = SDL_CreateRenderer(wnd.getWindow(), -1, SDL_RENDERER_ACCELERATED);
    if(!rend)
    {
        std::cerr << "SDL_CreateRenderer: " << SDL_GetError() << std::endl;
        SDL_Quit();
        std::exit(1);
    }

    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
    int imgInitted = IMG_Init(imgFlags);
    if((imgInitted & imgFlags) != imgFlags)
    {
        std::cerr << "IMG_Init: " << IMG_GetError() << std::endl;
        SDL_Quit();
        std::exit(1);
    }

    if(!TTF_WasInit())
    {
        if(TTF_Init() == -1)
        {
            std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
            SDL_Quit();
            std::exit(1);
        }
        font = TTF_OpenFont("res/PxPlus_IBM_VGA8.ttf", 24);
        if(!font)
        {
            std::cerr << "TTF_OpenFont: " << TTF_GetError() << std::endl;
        }
    }
}

SDL_Texture*
Render::createTexture(const char* filename)
{
    SDL_Surface *tmp = IMG_Load(filename);
    if(!tmp)
    {
        std::cerr << "IMG_Load: " << IMG_GetError() << std::endl;
        return NULL;
    }
    SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, tmp);
    if(!tex)
    {
        std::cerr << "SDL_CreateTextureFromSurface: " << SDL_GetError() << std::endl;
        return NULL;
    }

    return tex;
}
void
Render::setClear(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    SDL_SetRenderDrawColor(rend, r, g, b, a);
}

void
Render::clear()
{
    SDL_RenderClear(rend);
}

void
Render::renderBG()
{
    int w, h, ww, wh;
    Window& wnd = Window::getSingleton();
    Map& map = Map::getSingleton();

    wnd.getSize(ww, wh);

    SDL_QueryTexture(map.bg, NULL, NULL, &w, &h);
    SDL_Rect out;

    for(int y = 0; y < wh; y+=h)
    {
        for(int x = 0; x < ww; x+=w)
        {
            out.x = x;
            out.y = y;
            out.w = w;
            out.h = h;
            SDL_RenderCopy(rend, map.bg, NULL, &out);
        }
    }
}

void
Render::renderSnakes()
{
    Game& game = Game::getSingleton();
    for(auto snake : game.snakes)
    {
        for(auto segment = snake.segments.begin(); segment != snake.segments.end(); segment++)
        {
            SDL_RenderCopyEx(rend, segment->tex, NULL, &segment->rect, segment->rotation, NULL, segment->flip);
        }
    }
}

void
Render::renderPickups()
{
    for(auto pickup : map.pickups)
    {
        SDL_RenderCopy(rend, pickup.tex, NULL, &pickup.rect);
    }
}

void
Render::initText(Text *tex, const char* txt, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    SDL_Color color = {r, g, b, a};
    SDL_Surface *surface = TTF_RenderText_Solid(font, txt, color); 
    tex->tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_QueryTexture(tex->tex, NULL, NULL, &tex->w, &tex->h);
    SDL_FreeSurface(surface);
}

void
Render::renderButtons()
{
    Menu& menu = Menu::getSingleton();
    for(auto button : menu.buttons)
    {
        SDL_RenderCopy(rend, button.tex, NULL, &button.rect);
    }
}

void
Render::renderBorders()
{
    int ww, wh;
    Window& wnd = Window::getSingleton();
    Map& map = Map::getSingleton();

    wnd.getSize(ww, wh);
    setClear(64, 64, 64, 255);
    SDL_Rect top = {0, 0, ww, map.mapY};
    SDL_Rect left = {0, map.mapY, map.mapX, wh - map.mapY};
    SDL_Rect right = {ww - map.mapX, map.mapY, map.mapX, wh - map.mapY};
    SDL_Rect bottom = {map.mapX, wh - map.mapBottom, map.mapW, map.mapBottom};

    SDL_RenderFillRect(rend, &top);
    SDL_RenderFillRect(rend, &left);
    SDL_RenderFillRect(rend, &right);
    SDL_RenderFillRect(rend, &bottom);
}
void
Render::renderScores()
{
    Game& game = Game::getSingleton();
    if(!textP1Name.tex)
        initText(&textP1Name, "P1 SCORE: ", 255, 128, 32, 255);
    char p1Score[100] = {0};
    sprintf(p1Score, "%d", game.snakes[0].score);
    if(textP1Score.tex)
        SDL_DestroyTexture(textP1Score.tex);
    initText(&textP1Score, p1Score, 255, 255, 0, 255);
    renderText(&textP1Name, 30, 20, textP1Name.w, textP1Name.h);
    renderText(&textP1Score, textP1Name.out.x + textP1Name.w, textP1Name.out.y, textP1Score.w, textP1Name.h);
}

void
Render::renderText(Text *tex, int x, int y, int w, int h)
{
    tex->out.x = x;
    tex->out.y = y;
    tex->out.w = w;
    tex->out.h = h;
    SDL_RenderCopy(rend, tex->tex, NULL, &tex->out);
}

void
Render::renderMap()
{
    Map& map = Map::getSingleton();
    SDL_Rect texWallRect;
    SDL_QueryTexture(map.texWall, NULL, NULL, &texWallRect.w, &texWallRect.h);

    for(int i = 0; i < MAPH; i++)
    {
        for(int j = 0; j < MAPW; j++)
        {
            if(map.map[j][i] == TWALL)
            {
                texWallRect.x = (j * map.gridSize) + map.mapX;
                texWallRect.y = (i * map.gridSize) + map.mapY;
                SDL_RenderCopy(rend, map.texWall, NULL, &texWallRect);
            }
        }
    }
}

void
Render::show()
{
    SDL_RenderPresent(rend);
}
