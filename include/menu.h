#ifndef MENU_H
#define MENU_H

#include <vector>
#include <iostream>
#include <SDL.h>
#include <functional>

struct Button
{
    std::function<void()> clickFunction;
    SDL_Rect rect;
    SDL_Texture* tex;
    SDL_Color color, colorHover;
    bool clicked = false;
};

class Menu
{
public:
    std::vector<Button> buttonsMenuMain;

    Menu(const Menu&) = delete;
    static Menu& getSingleton()
    {
        return s_Menu;
    }
void createButton(
        std::function<void()> callback,
        const char* txt,
        int x, int y, int w, int h,
        SDL_Color color,
        SDL_Color colorTxt);
void checkButtonMenuMain();
void buttonCallbackSetBG();
private:
    Menu(){}
    static Menu s_Menu;
};

#endif // MENU_H
