#ifndef MENU_H
#define MENU_H

#include <vector>
#include <iostream>
#include <SDL.h>
#include <functional>

enum ButtonTrigger
{
    BPRESS, BRELEASE
};

struct Button
{
    std::function<void()> clickFunction;
    ButtonTrigger trigger = BRELEASE;
    SDL_Rect rect;
    SDL_Texture* tex;
    SDL_Color color;
    SDL_Color tint;
    bool active = false;
};

class Menu
{
public:
    std::vector<Button> buttons;
    SDL_Texture* btnMenuBig, *btnMenuSmall, *btnEditor;

    Menu(const Menu&) = delete;
    static Menu& getSingleton()
    {
        return s_Menu;
    }
void setButtonColorTxt(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void createButton(
        std::function<void()> callback, ButtonTrigger trigger,
        const char* txt,
        int x, int y, int w, int h,
        int emboss);
Button* checkButtons();
void buttonCallbackSetBG();
void clearButtons();
private:
    Menu(){};
    static Menu s_Menu;
    SDL_Color colorTxt = {255, 255, 255, 255};
};

#endif // MENU_H
