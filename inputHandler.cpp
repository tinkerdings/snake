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
        if((button = menu.checkButtons()) && button->active)
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
