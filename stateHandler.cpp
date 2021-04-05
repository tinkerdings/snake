#include "stateHandler.h"
#include "game.h"
#include "menu.h"

StateHandler StateHandler::s_StateHandler;

void
StateHandler::setState(State set)
{
    current = set;
}

void
StateHandler::run()
{
    InputHandler& input = InputHandler::getSingleton();
    input.pollInput();
    input.checkQuit();
    switch(current)
    {
    case(MENU):
    {
        stateMenu();
        break;
    }
    case(PLAY):
    {
        statePlay();
        break;
    }
    case(RESTART):
    {
        stateRestart();
        break;
    }
    }
}

void
StateHandler::stateMenu()
{
    Menu& menu = Menu::getSingleton();
    Render& rend = Render::getSingleton();
    Window& wnd = Window::getSingleton();
    int ww, wh;
    wnd.getSize(ww, wh);
    if(!menu.buttonsMenuMain.size())
    {
        SDL_Color color = {64, 64, 255, 255};
        SDL_Color colorTxt = {0, 255, 64, 255};

    menu.createButton(
            std::bind(&StateHandler::setState, this, PLAY),
            "PLAY",
            ww/2 - 200, 200-50, 400, 100,
            color, colorTxt);
    }
    menu.checkButtonMenuMain();

    rend.setBG(64, 64, 64, 255);
    rend.clear();
    rend.renderButtonsMenuMain();
    rend.show();
}
void
StateHandler::statePlay()
{
    Render& rend = Render::getSingleton();
    Game& game = Game::getSingleton();
    InputHandler& input = InputHandler::getSingleton();
    Menu& menu = Menu::getSingleton();

    if(!game.started)
		game.init();
    
    input.inputPlay();

    game.update();

    rend.setBG(64, 64, 64, 255);
    rend.clear();

    rend.renderSnakes();
    rend.renderPickups();
    rend.renderUI();

    rend.show();
}

void
StateHandler::stateRestart()
{
    Game& game = Game::getSingleton();
    game.started = false;
    current = PLAY;
}
