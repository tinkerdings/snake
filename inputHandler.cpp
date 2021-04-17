#include "inputHandler.h"
#include <iostream>
#include <cstdio>
#include "game.h"
#include "menu.h"

InputHandler InputHandler::s_InputHandler;

void
InputHandler::commonInput()
{
    if(e.type == SDL_QUIT)
        wnd.quit();
    updateMouse();
}

void
InputHandler::pollInput()
{
    SDL_PollEvent(&e);
}

void
InputHandler::inputPlay()
{
    Game& game = Game::getSingleton();
    StateHandler& state = StateHandler::getSingleton();

    if(keyPress(SDLK_ESCAPE))
    {
        state.setState(MENU);
        return;
    }
    if(keyPress(SDLK_UP) || keyPress(SDLK_w))
    {
        if(game.snakes[0].dirAvailable(DIR_UP))
			game.snakes[0].setDirection(DIR_UP);
    }
    if(keyPress(SDLK_DOWN) || keyPress(SDLK_s))
    {
        if(game.snakes[0].dirAvailable(DIR_DOWN))
			game.snakes[0].setDirection(DIR_DOWN);
    }
    if(keyPress(SDLK_LEFT) || keyPress(SDLK_a))
    {
        if(game.snakes[0].dirAvailable(DIR_LEFT))
			game.snakes[0].setDirection(DIR_LEFT);
    }
    if(keyPress(SDLK_RIGHT) || keyPress(SDLK_d))
    {
        if(game.snakes[0].dirAvailable(DIR_RIGHT))
			game.snakes[0].setDirection(DIR_RIGHT);
    }
}

void
InputHandler::inputCreate()
{
    Map& map = Map::getSingleton();
    Menu& menu = Menu::getSingleton();
    Window& wnd = Window::getSingleton();
    int ww, wh;
    wnd.getSize(ww, wh);

    inputButtons();

    if(
        (mouseX >= map.mapX) && (mouseX < (map.mapX + map.mapW)) &&
        (mouseY >= map.mapY) && (mouseY < (map.mapY + map.mapH)))
    {
        map.editorValidPlacement = true;
        if((map.editorActiveTile == TP1START) || (map.editorActiveTile == TP2START))
        {
            if((mouseY >= map.mapY+map.mapH-map.gridSize) && map.editorRotation == 0)
                map.editorValidPlacement = false;
            if((mouseX < map.mapX+map.gridSize) && map.editorRotation == 90)
                map.editorValidPlacement = false;
            if((mouseY < map.mapY+map.gridSize) && map.editorRotation == 180)
                map.editorValidPlacement = false;
            if((mouseX >= map.mapX+map.mapW-map.gridSize) && map.editorRotation == 270)
                map.editorValidPlacement = false;
        }

        if(mouseDown(SDL_BUTTON_LEFT))
        {
            switch(map.editorActiveTile)
            {
            case(TWALL):
            {
                map.setTile(mouseX, mouseY, TWALL);

                break;
            }
            case(TP1START):
            {
                map.setTile(mouseX, mouseY, TP1START);

                break;
            }
            case(TP2START):
            {
                map.setTile(mouseX, mouseY, TP2START);

                break;
            }
            }
        }
        if(mouseDown(SDL_BUTTON_RIGHT))
        {
            map.setTile(mouseX, mouseY, TEMPTY);
        }
    }
    else
    {
        map.editorValidPlacement = false;
    }
    if(mouseScroll(SCROLLUP))
    {
        map.nextEditorTile();
    }
    if(mouseScroll(SCROLLDOWN))
    {
        map.prevEditorTile();
    }
    if(keyPress(SDLK_r))
    {
        map.editorRotate();
    }
    if(map.savingMap)
    {
        getInputLine(16);
    }
}

void
InputHandler::getInputLine(int maxChars)
{
    Map& map = Map::getSingleton();
    Menu& menu = Menu::getSingleton();
    Render& rend = Render::getSingleton();

    static int inputted = 0;

    if(keyPress(SDLK_RETURN))
    {
        if(inputted)
        {
            std::string name(map.editorSaveName.begin(), map.editorSaveName.end());
            map.saveMap(name);
            menu.buttons.pop_back();
            map.savingMap = false;
        }
    }
    if(keyPress(SDLK_ESCAPE))
    {
        menu.buttons.pop_back();
        map.editorSaveName.clear();
        inputted = 0;
        map.savingMap = false;
    }
    if(keyPress(SDLK_BACKSPACE))
    {
        if(inputted > 0)
        {
            map.editorSaveName.clear();
            inputted = 0;
        }
    }
    if(inputted < maxChars)
    {
        if(keyPress(SDLK_a))
        {
            map.editorSaveName.push_back('a');
            inputted++;
        }
        if(keyPress(SDLK_b))
        {
            map.editorSaveName.push_back('b');
            inputted++;
        }
        if(keyPress(SDLK_c))
        {
            map.editorSaveName.push_back('c');
            inputted++;
        }
        if(keyPress(SDLK_d))
        {
            map.editorSaveName.push_back('d');
            inputted++;
        }
        if(keyPress(SDLK_e))
        {
            map.editorSaveName.push_back('e');
            inputted++;
        }
        if(keyPress(SDLK_f))
        {
            map.editorSaveName.push_back('f');
            inputted++;
        }
        if(keyPress(SDLK_g))
        {
            map.editorSaveName.push_back('g');
            inputted++;
        }
        if(keyPress(SDLK_h))
        {
            map.editorSaveName.push_back('h');
            inputted++;
        }
        if(keyPress(SDLK_i))
        {
            map.editorSaveName.push_back('i');
            inputted++;
        }
        if(keyPress(SDLK_j))
        {
            map.editorSaveName.push_back('j');
            inputted++;
        }
        if(keyPress(SDLK_k))
        {
            map.editorSaveName.push_back('k');
            inputted++;
        }
        if(keyPress(SDLK_l))
        {
            map.editorSaveName.push_back('l');
            inputted++;
        }
        if(keyPress(SDLK_m))
        {
            map.editorSaveName.push_back('m');
            inputted++;
        }
        if(keyPress(SDLK_n))
        {
            map.editorSaveName.push_back('n');
            inputted++;
        }
        if(keyPress(SDLK_o))
        {
            map.editorSaveName.push_back('o');
            inputted++;
        }
        if(keyPress(SDLK_p))
        {
            map.editorSaveName.push_back('p');
            inputted++;
        }
        if(keyPress(SDLK_q))
        {
            map.editorSaveName.push_back('q');
            inputted++;
        }
        if(keyPress(SDLK_r))
        {
            map.editorSaveName.push_back('r');
            inputted++;
        }
        if(keyPress(SDLK_s))
        {
            map.editorSaveName.push_back('s');
            inputted++;
        }
        if(keyPress(SDLK_t))
        {
            map.editorSaveName.push_back('t');
            inputted++;
        }
        if(keyPress(SDLK_u))
        {
            map.editorSaveName.push_back('u');
            inputted++;
        }
        if(keyPress(SDLK_v))
        {
            map.editorSaveName.push_back('v');
            inputted++;
        }
        if(keyPress(SDLK_w))
        {
            map.editorSaveName.push_back('w');
            inputted++;
        }
        if(keyPress(SDLK_x))
        {
            map.editorSaveName.push_back('x');
            inputted++;
        }
        if(keyPress(SDLK_y))
        {
            map.editorSaveName.push_back('y');
            inputted++;
        }
        if(keyPress(SDLK_z))
        {
            map.editorSaveName.push_back('z');
            inputted++;
        }
    }
}

void
InputHandler::inputMenu()
{
    inputButtons();
}

void
InputHandler::inputButtons()
{
    Menu& menu = Menu::getSingleton();
    static Button *button = NULL;
    if(mousePress(SDL_BUTTON_LEFT))
    {
        if((button = menu.checkButtons()))
        {
            if(button->trigger == BPRESS)
            {
                button->clickFunction();
            }
            else
            {
                button->active = true;
            }
        }
    }
    if(mouseRelease(SDL_BUTTON_LEFT))
    {
        if((button = menu.checkButtons()) && button->clickFunction != NULL && button->active)
        {
            if(button->trigger == BRELEASE)
            {
                button->clickFunction();
            }
        }
    }
}

// [ INPUT CHECKING METHODS ]
bool
InputHandler::keyPress(int sdlKeycode)
{
    return (e.type == SDL_KEYDOWN) && ((!e.key.repeat) && (sdlKeycode == e.key.keysym.sym));
}

bool
InputHandler::keyHold(int sdlKeycode)
{
    SDL_PumpEvents();
    return keyboardState[SDL_GetScancodeFromKey(sdlKeycode)];
}

bool
InputHandler::keyRelease(int sdlKeycode)
{
    return (e.type == SDL_KEYUP) && (sdlKeycode == e.key.keysym.sym);
}

void
InputHandler::updateMouse()
{
    Window& wnd = Window::getSingleton();
    int ww, wh;
    static int i = 0;
    wnd.getSize(ww, wh);

    scrollDown[0] = scrollDown[1];
    scrollUp[0] = scrollUp[1];
    scrollUp[1] = false;
    scrollDown[1] = false;

    if((e.type == SDL_MOUSEMOTION))
    {
        mouseX = e.button.x;
        mouseY = e.button.y;
    }

    switch(e.type)
    {
    case(SDL_MOUSEBUTTONDOWN):
    {
        switch(e.button.button)
        {
        case(SDL_BUTTON_LEFT):
        {
            mouseButton[0].prevDown = mouseButton[0].curDown;
            mouseButton[0].curDown = true;
            break;
        }
        case(SDL_BUTTON_RIGHT):
        {
            mouseButton[1].prevDown = mouseButton[1].curDown;
            mouseButton[1].curDown = true;
            break;
        }
        }
        break;
    }
    case(SDL_MOUSEBUTTONUP):
    {
        switch(e.button.button)
        {
        case(SDL_BUTTON_LEFT):
        {
            mouseButton[0].prevDown = mouseButton[0].curDown;
            mouseButton[0].curDown = false;
            break;
        }
        case(SDL_BUTTON_RIGHT):
        {
            mouseButton[1].prevDown = mouseButton[1].curDown;
            mouseButton[1].curDown = false;
            break;
        }
        }
        break;
    }
    case(SDL_MOUSEWHEEL):
    {
        if(e.wheel.y > 0)
        {
            scrollUp[1] = true;
        }
        if(e.wheel.y < 0)
        {
            scrollDown[1] = true;
        }
        e.wheel.y = 0;
        break;
    }
    }

}
bool
InputHandler::mousePress(int sdlMousecode)
{
    switch(sdlMousecode)
    {
    case(SDL_BUTTON_LEFT):
    {
        if(!mouseButton[0].prevDown && mouseButton[0].curDown)
        {
            return true;
        }
        break;
    }
    case(SDL_BUTTON_RIGHT):
    {
        if(!mouseButton[1].prevDown && mouseButton[1].curDown)
        {
            return true;
        }
        break;
    }
    }
    return false;
}

bool
InputHandler::mouseDown(int sdlMousecode)
{
    switch(sdlMousecode)
    {
    case(SDL_BUTTON_LEFT):
    {
        if(mouseButton[0].curDown)
        {
            return true;
        }
        break;
    }
    case(SDL_BUTTON_RIGHT):
    {
        if(mouseButton[1].curDown)
        {
            return true;
        }
        break;
    }
    }
    return false;
}

bool
InputHandler::mouseRelease(int sdlMousecode)
{
    switch(sdlMousecode)
    {
    case(SDL_BUTTON_LEFT):
    {
        if(mouseButton[0].prevDown && !mouseButton[0].curDown)
        {
            return true;
        }
        break;
    }
    case(SDL_BUTTON_RIGHT):
    {
        if(mouseButton[1].prevDown && !mouseButton[1].curDown)
        {
            return true;
        }
        break;
    }
    }
    return false;
}

bool
InputHandler::mouseScroll(Mscroll scroll)
{
    switch(scroll)
    {
    case(SCROLLUP):
    {
        return (scrollUp[1] && !scrollUp[0]);
    }
    case(SCROLLDOWN):
    {
        return (scrollDown[1] && !scrollDown[0]);
    }
    }

    return false;
}
