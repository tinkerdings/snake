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
    SDL_Color color;
};

class Menu
{
public:
    std::vector<Button> buttons;

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
bool checkButtons();
void buttonCallbackSetBG();
void clearButtons();
private:
    Menu(){}
    static Menu s_Menu;
};

#endif // MENU_H
