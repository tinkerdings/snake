#include "menu.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <thread>
#include "render.h"
#include "inputHandler.h"

void
Menu::setButtonColorBG(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
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
        std::function<void()> callback,
        const char* txt,
        int x, int y, int w, int h)
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
    button.color = color;
    SDL_FillRect(surfButton, NULL, SDL_MapRGBA(surfButton->format, button.color.r, button.color.g, button.color.b, button.color.a));
    SDL_Surface *surfTxt = TTF_RenderText_Solid(rend.font, txt, colorTxt);
    if(!surfTxt)
    {
        std::cerr << "TTF_RenderText_Solid: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Rect rectTxt = {(button.rect.w/2)-(surfTxt->w/2), (button.rect.h/2)-(surfTxt->h/2), button.rect.w, button.rect.h};
    if(SDL_BlitSurface(surfTxt, NULL, surfButton, &rectTxt) < 0)
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

    SDL_FreeSurface(surfButton);
    SDL_FreeSurface(surfTxt);

    buttons.push_back(button);
}

bool
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
            button->clickFunction();
            return true;
        }
    }
    return false;
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
