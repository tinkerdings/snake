#include "menu.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <thread>
#include <fstream>
#include <sstream>
#include "render.h"
#include "inputHandler.h"

Menu Menu::s_Menu;

void
Menu::setButtonColorTxt(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    colorTxt.r = r;
    colorTxt.g = g;
    colorTxt.b = b;
    colorTxt.a = a;
}

void
Menu::setButtonColorTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    colorTint.r = r;
    colorTint.g = g;
    colorTint.b = b;
    colorTint.a = a;
}

void
Menu::createButton(
        std::function<void()> callback, ButtonTrigger trigger,
        const char* txt,
        int x, int y, int w, int h,
        int emboss,
        ButtonType type)
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
        rend.fontInput = TTF_OpenFont("res/PxPlus_IBM_VGA8.ttf", 16);
        if(!rend.font || !rend.fontInput)
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

    SDL_FillRect(surfButton, NULL, SDL_MapRGBA(surfButton->format, 96, 96, 96, 255));
    for(int i = 0; i < emboss; i++)
    {
        SDL_Rect shadowL = {i, i, 1, h-i};
        SDL_Rect shadowB = {emboss, h-i, w-emboss-i, 1};
        SDL_Rect lightT = {i, i, w-i, 1};
        SDL_Rect lightR = {w-i, 0, 1, h-i};
        SDL_FillRect(surfButton, &shadowL, SDL_MapRGBA(surfButton->format, 32, 32, 32, 255));
        SDL_FillRect(surfButton, &shadowB, SDL_MapRGBA(surfButton->format, 32, 32, 32, 255));
        SDL_FillRect(surfButton, &lightT, SDL_MapRGBA(surfButton->format, 196, 196, 196, 255));
        SDL_FillRect(surfButton, &lightR, SDL_MapRGBA(surfButton->format, 196, 196, 196, 255));
    }

    if(type == BTPREVIEW)
    {
        std::stringstream ss;
        ss << "./maps/" << std::string(txt) << ".map";
        std::string filename = ss.str();
        std::ifstream file(filename);
        if(!file)
        {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }

        char c;
        while(!file.eof())
        {
            file.read(&c, 1);
            if(c == 0)
            {
                std::cout << "empty!" << std::endl;
            }
        }
    }

    SDL_Color colorTxtBase = colorTxt;
    SDL_Color colorTxtShadow = {21, 10, 36, 255};
    SDL_Color colorTxtLight = {90, 14, 194, 255};
    SDL_Surface *surfTxtBase;
    SDL_Surface *surfTxtShadow;
    SDL_Surface *surfTxtLight;

    switch(type)
    {
    case(BTBUTTON):
    {
        surfTxtBase = TTF_RenderText_Solid(rend.font, txt, colorTxtBase);
        surfTxtShadow = TTF_RenderText_Solid(rend.font, txt, colorTxtShadow);
        surfTxtLight = TTF_RenderText_Solid(rend.font, txt, colorTxtLight);

        break;
    }
    case(BTPREVIEW): // FALLTHROUGH
    case(BTINPUT):
    {
        surfTxtBase = TTF_RenderText_Solid(rend.fontInput, txt, colorTxtBase);
        surfTxtShadow = TTF_RenderText_Solid(rend.fontInput, txt, colorTxtShadow);
        surfTxtLight = TTF_RenderText_Solid(rend.fontInput, txt, colorTxtLight);

        break;
    }
    }

    if(!surfTxtBase || !surfTxtShadow || !surfTxtLight)
    {
        std::cerr << "TTF_RenderText_Solid: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Rect rectTxtBase;
    SDL_Rect rectTxtShadowR;
    SDL_Rect rectTxtShadowU;
    SDL_Rect rectTxtShadowUR;
    SDL_Rect rectTxtShadowUL;

    SDL_Rect rectTxtLightL;
    SDL_Rect rectTxtLightD;
    SDL_Rect rectTxtLightDL;
    SDL_Rect rectTxtLightDR;

    switch(type)
    {
    case(BTBUTTON):
    {
        rectTxtBase.x = (button.rect.w/2)-(surfTxtBase->w/2);
        rectTxtBase.y = (button.rect.h/2)-(surfTxtBase->h/2);
        rectTxtBase.w = button.rect.w;
        rectTxtBase.h = button.rect.h;

        rectTxtShadowR = {(button.rect.w/2)-(surfTxtShadow->w/2) + 2, (button.rect.h/2)-(surfTxtShadow->h/2), button.rect.w, button.rect.h};
        rectTxtShadowU = {(button.rect.w/2)-(surfTxtShadow->w/2), (button.rect.h/2)-(surfTxtShadow->h/2) - 2, button.rect.w, button.rect.h};
        rectTxtShadowUR = {(button.rect.w/2)-(surfTxtShadow->w/2) + 2, (button.rect.h/2)-(surfTxtShadow->h/2) - 2, button.rect.w, button.rect.h};
        rectTxtShadowUL = {(button.rect.w/2)-(surfTxtShadow->w/2) - 2, (button.rect.h/2)-(surfTxtShadow->h/2) - 2, button.rect.w, button.rect.h};

        rectTxtLightL = {(button.rect.w/2)-(surfTxtLight->w/2) - 2, (button.rect.h/2)-(surfTxtLight->h/2), button.rect.w, button.rect.h};
        rectTxtLightD = {(button.rect.w/2)-(surfTxtLight->w/2), (button.rect.h/2)-(surfTxtLight->h/2) + 2, button.rect.w, button.rect.h};
        rectTxtLightDL = {(button.rect.w/2)-(surfTxtLight->w/2) - 2, (button.rect.h/2)-(surfTxtLight->h/2) + 2, button.rect.w, button.rect.h};
        rectTxtLightDR = {(button.rect.w/2)-(surfTxtLight->w/2) + 2, (button.rect.h/2)-(surfTxtLight->h/2) + 2, button.rect.w, button.rect.h};

        break;
    }
    case(BTPREVIEW): // FALLTHROUGH
    case(BTINPUT):
    {
        rectTxtBase.x = (button.rect.w/2)-(surfTxtBase->w/2);
        rectTxtBase.y = (button.rect.h/5)-(surfTxtBase->h/2);
        rectTxtBase.w = button.rect.w;
        rectTxtBase.h = button.rect.h;

        rectTxtShadowR = {(button.rect.w/2)-(surfTxtShadow->w/2) + 2, (button.rect.h/5)-(surfTxtShadow->h/2), button.rect.w, button.rect.h};
        rectTxtShadowU = {(button.rect.w/2)-(surfTxtShadow->w/2), (button.rect.h/5)-(surfTxtShadow->h/2) - 2, button.rect.w, button.rect.h};
        rectTxtShadowUR = {(button.rect.w/2)-(surfTxtShadow->w/2) + 2, (button.rect.h/5)-(surfTxtShadow->h/2) - 2, button.rect.w, button.rect.h};
        rectTxtShadowUL = {(button.rect.w/2)-(surfTxtShadow->w/2) - 2, (button.rect.h/5)-(surfTxtShadow->h/2) - 2, button.rect.w, button.rect.h};

        rectTxtLightL = {(button.rect.w/2)-(surfTxtLight->w/2) - 2, (button.rect.h/5)-(surfTxtLight->h/2), button.rect.w, button.rect.h};
        rectTxtLightD = {(button.rect.w/2)-(surfTxtLight->w/2), (button.rect.h/5)-(surfTxtLight->h/2) + 2, button.rect.w, button.rect.h};
        rectTxtLightDL = {(button.rect.w/2)-(surfTxtLight->w/2) - 2, (button.rect.h/5)-(surfTxtLight->h/2) + 2, button.rect.w, button.rect.h};
        rectTxtLightDR = {(button.rect.w/2)-(surfTxtLight->w/2) + 2, (button.rect.h/5)-(surfTxtLight->h/2) + 2, button.rect.w, button.rect.h};

        break;
    }
    }

    if(
            (SDL_BlitSurface(surfTxtLight,  NULL, surfButton, &rectTxtLightL)   < 0) ||
            (SDL_BlitSurface(surfTxtLight,  NULL, surfButton, &rectTxtLightD)   < 0) ||
            (SDL_BlitSurface(surfTxtLight,  NULL, surfButton, &rectTxtLightDL)  < 0) ||
            (SDL_BlitSurface(surfTxtLight,  NULL, surfButton, &rectTxtLightDR)  < 0) ||
            (SDL_BlitSurface(surfTxtShadow, NULL, surfButton, &rectTxtShadowR)  < 0) ||
            (SDL_BlitSurface(surfTxtShadow, NULL, surfButton, &rectTxtShadowU)  < 0) ||
            (SDL_BlitSurface(surfTxtShadow, NULL, surfButton, &rectTxtShadowUR) < 0) ||
            (SDL_BlitSurface(surfTxtShadow, NULL, surfButton, &rectTxtShadowUL) < 0) ||
            (SDL_BlitSurface(surfTxtBase,   NULL, surfButton, &rectTxtBase)     < 0))
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
    SDL_FreeSurface(surfTxtLight);
    SDL_FreeSurface(surfTxtBase);

    switch(type)
    {
    case(BTBUTTON): // FALLTHROUGH
    case(BTINPUT):
    {
        buttons.push_back(button);

        break;
    }
    case(BTPREVIEW):
    {
        mapPreviews.push_back(button);
    }
    }
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

Button*
Menu::checkMapPreviews()
{
    InputHandler& input = InputHandler::getSingleton();
    for(auto preview = mapPreviews.begin(); preview != mapPreviews.end(); preview++)
    {
        if(!mapPreviews.size())
            break;
        if(((input.e.button.x >= preview->rect.x)&&(input.e.button.x <= (preview->rect.x+preview->rect.w))) &&
           ((input.e.button.y >= preview->rect.y)&&(input.e.button.y <= (preview->rect.y+preview->rect.h))))
        {
            return &(*preview);
        }
        else
        {
            preview->active = false;
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
