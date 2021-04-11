#include "menu.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <thread>
#include "render.h"
#include "inputHandler.h"

void
Menu::setButtonColorTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    colorTint.r = r;
    colorTint.g = g;
    colorTint.b = b;
    colorTint.a = a;
}
void
Menu::setButtonColorTxt(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    colorTxt.r = r;
    colorTxt.g = g;
    colorTxt.b = b;
    colorTxt.a = a;
}
Menu Menu::s_Menu;
void
Menu::createButton(
        std::function<void()> callback, ButtonTrigger trigger,
        const char* txt,
        int x, int y, int w, int h,
        int emboss)
{
    Render& rend = Render::getSingleton();
    if(!TTF_WasInit())
    {
        if(TTF_Init() == -1)
        {
            std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
            SDL_Quit();
            std::exit(1);
        }
        rend.font = TTF_OpenFont("res/PxPlus_IBM_VGA8.ttf", 24);
        if(!rend.font)
        {
            std::cerr << "TTF_OpenFont: " << TTF_GetError() << std::endl;
        }
    }

    Button button; 
    button.rect = {x, y, w, h};
    SDL_Surface *surfButton = SDL_CreateRGBSurface(0, button.rect.w, button.rect.h, 32, 0, 0, 0, 0);
    if(!surfButton)
    {
        std::cerr << "SDL_CreateRGBSurface: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_FillRect(surfButton, NULL, SDL_MapRGBA(surfButton->format, 48, 6, 105, 255));
    for(int i = 0; i < emboss; i++)
    {
        SDL_Rect shadowL = {i, i, 1, h-i};
        SDL_Rect shadowB = {emboss, h-i, w-emboss-i, 1};
        SDL_Rect lightT = {i, i, w-i, 1};
        SDL_Rect lightR = {w-i, 0, 1, h-i};
        SDL_FillRect(surfButton, &shadowL, SDL_MapRGBA(surfButton->format, 21, 10, 36, 255));
        SDL_FillRect(surfButton, &shadowB, SDL_MapRGBA(surfButton->format, 21, 10, 36, 255));
        SDL_FillRect(surfButton, &lightT, SDL_MapRGBA(surfButton->format, 90, 14, 194, 255));
        SDL_FillRect(surfButton, &lightR, SDL_MapRGBA(surfButton->format, 90, 14, 194, 255));
    }

    SDL_Color colorTxtShadow = {21, 10, 36, 255};
    SDL_Color colorTxtBase = colorTxt;
    SDL_Surface *surfTxtShadow = TTF_RenderText_Solid(rend.font, txt, colorTxtShadow);
    SDL_Surface *surfTxtBase = TTF_RenderText_Solid(rend.font, txt, colorTxtBase);
    if(!surfTxtBase || !surfTxtShadow)
    {
        std::cerr << "TTF_RenderText_Solid: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Rect rectTxtBase = {(button.rect.w/2)-(surfTxtBase->w/2), (button.rect.h/2)-(surfTxtBase->h/2), button.rect.w, button.rect.h};
    SDL_Rect rectTxtShadowR = {(button.rect.w/2)-(surfTxtShadow->w/2) + 2, (button.rect.h/2)-(surfTxtShadow->h/2), button.rect.w, button.rect.h};
    SDL_Rect rectTxtShadowU = {(button.rect.w/2)-(surfTxtShadow->w/2), (button.rect.h/2)-(surfTxtShadow->h/2) - 2, button.rect.w, button.rect.h};
    SDL_Rect rectTxtShadowUR = {(button.rect.w/2)-(surfTxtShadow->w/2) + 2, (button.rect.h/2)-(surfTxtShadow->h/2) - 2, button.rect.w, button.rect.h};
    if(
            (SDL_BlitSurface(surfTxtShadow, NULL, surfButton, &rectTxtShadowR) < 0) ||
            (SDL_BlitSurface(surfTxtShadow, NULL, surfButton, &rectTxtShadowU) < 0) ||
            (SDL_BlitSurface(surfTxtShadow, NULL, surfButton, &rectTxtShadowUR) < 0) ||
            (SDL_BlitSurface(surfTxtBase, NULL, surfButton, &rectTxtBase) < 0))
    {
        std::cerr << "SDL_BlitSurface: " << SDL_GetError() << std::endl;
        return;
    }
    if(rend.rend == NULL)
        std::cerr << "Renderer not initialized" << std::endl;
    button.tex = SDL_CreateTextureFromSurface(rend.rend, surfButton);
    if(!button.tex)
    {
        std::cerr << "SDL_CreateTextureFromSurface: " << SDL_GetError() << std::endl;
        return;
    }

    button.clickFunction = callback;
    button.trigger = trigger;

    SDL_FreeSurface(surfButton);
    SDL_FreeSurface(surfTxtShadow);
    SDL_FreeSurface(surfTxtBase);

    buttons.push_back(button);
}

Button*
Menu::checkButtons()
{
    InputHandler& input = InputHandler::getSingleton();
    for(auto button = buttons.begin(); button != buttons.end(); button++)
    {
        if(!buttons.size())
            break;
        if(((input.e.button.x >= button->rect.x)&&(input.e.button.x <= (button->rect.x+button->rect.w))) &&
           ((input.e.button.y >= button->rect.y)&&(input.e.button.y <= (button->rect.y+button->rect.h))))
        {
            return &(*button);
        }
        else
        {
            button->active = false;
        }
    }
    return NULL;
}

void
Menu::clearButtons()
{
    if(buttons.size())
    {
        for(auto button = buttons.begin(); button != buttons.end(); button++)
        {
            SDL_DestroyTexture(button->tex);
            button->clickFunction = NULL;
        }
        buttons.clear();
    }
}
