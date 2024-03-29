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

enum ButtonType
{
    BTBUTTON, BTPREVIEW, BTINPUT
};

struct Button
{
    std::function<void()> clickFunction;
    ButtonTrigger trigger = BRELEASE;
    SDL_Rect rect;
    SDL_Texture* tex;
    SDL_Color color;
    SDL_Color tint;
    ButtonType type = BTBUTTON;
    bool active = false;
};

class Menu
{
public:
    std::vector<Button> buttons;
    std::vector<Button> mapPreviews;
    int mapPreviewW = 200;
    int mapPreviewH = 200;
    int mapPreviewSpacing = 20;
    int mapPreviewScrollOffset = 0;

    Menu(const Menu&) = delete;
    static Menu& getSingleton()
    {
        return s_Menu;
    }
    void setButtonColorTxt(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void setButtonColorTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void createButton(
        std::function<void()> callback, ButtonTrigger trigger,
        const char* txt,
        int x, int y, int w, int h,
        int emboss,
        ButtonType type);
    Button* checkButtons();
    Button* checkMapPreviews();
    void buttonCallbackSetBG();
    void clearButtons();
    void movePreviews(int moveY);
private:
    Menu(){};
    static Menu s_Menu;
    SDL_Color colorTxt = {255, 255, 255, 255};
    SDL_Color colorTint = {255, 0, 0, 64};
};

#endif // MENU_H
