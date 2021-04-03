#include <cstdio>
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "render.h"
#include "game.h"
#include "snake.h"

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

void
Render::setBG(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    SDL_SetRenderDrawColor(rend, r, g, b, a);
}

void
Render::clear()
{
    SDL_RenderClear(rend);
}

void
Render::renderSnakes()
{
    Game& game = Game::getSingleton();
    for(auto snake : game.snakes)
    {
        for(auto segment : snake.segments)
        {
            setBG(segment.color.r, segment.color.g, segment.color.b, segment.color.a);
            SDL_RenderFillRect(rend, &segment.rect);
        }
    }
}

void
Render::renderPickups()
{
    for(auto pickup : map.pickups)
    {
        setBG(pickup.color.r, pickup.color.g, pickup.color.b, pickup.color.a);
        SDL_RenderFillRect(rend, &pickup.rect);
    }
}

void
Render::renderUI()
{
    renderScores();
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
Render::show()
{
    SDL_RenderPresent(rend);
}
