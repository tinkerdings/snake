#include "stateHandler.h"
#include "game.h"
#include "menu.h"
#include "map.h"

StateHandler StateHandler::s_StateHandler;

void
StateHandler::setState(State set)
{
    Menu& menu = Menu::getSingleton();
    InputHandler& input = InputHandler::getSingleton();
    menu.clearButtons();
    current = set;
}

void
StateHandler::run()
{
    InputHandler& input = InputHandler::getSingleton();
    input.pollInput();
    input.commonInput();
    switch(current)
    {
    case(MENU):
    {
        stateMenu();
        break;
    }
    case(MENU_CREATE):
    {
        stateMenuCreate();
        break;
    }
    case(PLAY):
    {
        statePlay();
        break;
    }
    case(CREATE):
    {
        stateCreate();
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
    InputHandler& input = InputHandler::getSingleton();
    Render& rend = Render::getSingleton();
    Window& wnd = Window::getSingleton();
    int ww, wh;

    wnd.getSize(ww, wh);

    rend.renderBG();

    if(!menu.buttons.size())
    {
        menu.setButtonColorBG(64, 64, 255, 255);
        menu.setButtonColorTxt(0, 255, 64, 255);

        menu.createButton(
            std::bind(&StateHandler::setState, this, PLAY), BRELEASE,
            "PLAY",
            ww/2 - 200, 140, 400, 100);

        menu.createButton(
            std::bind(&StateHandler::setState, this, MENU_CREATE), BRELEASE,
            "CREATE",
            ww/2 - 200, 300, 400, 100);

        menu.createButton(
            std::bind(&Window::quit, &wnd), BRELEASE,
            "QUIT",
            20, wh - 60, 80, 40);
    }

    rend.renderButtons();
    input.inputMenu();
    rend.show();

}

void
StateHandler::stateMenuCreate()
{
    Menu& menu = Menu::getSingleton();
    Render& rend = Render::getSingleton();
    InputHandler& input = InputHandler::getSingleton();
    Window& wnd = Window::getSingleton();
    int ww, wh;
    wnd.getSize(ww, wh);

    rend.renderBG();

    if(!menu.buttons.size())
    {
        menu.setButtonColorBG(64, 64, 255, 255);
        menu.setButtonColorTxt(0, 255, 64, 255);

        menu.createButton(
            std::bind(&StateHandler::setState, this, CREATE), BRELEASE,
            "+",
            ww/2 - 200, 140, 400, 100);

        menu.createButton(
            std::bind(&StateHandler::setState, this, MENU), BRELEASE,
            "BACK",
            ww/2 - 200, 300, 400, 100);
    }
    rend.renderButtons();
    input.inputMenu();

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

    rend.renderBG();

    rend.renderSnakes();
    rend.renderPickups();
    rend.renderUI();

    rend.show();
}

void
StateHandler::stateCreate()
{
    Render& rend = Render::getSingleton();
    InputHandler& input = InputHandler::getSingleton();
    Menu& menu = Menu::getSingleton();
    Map& map = Map::getSingleton();
    Window& wnd = Window::getSingleton();
    int ww, wh;

    wnd.getSize(ww, wh);

    if(!menu.buttons.size())
    {
        menu.setButtonColorBG(64, 64, 255, 255);
        menu.setButtonColorTxt(0, 255, 64, 255);

        menu.createButton(
            std::bind(&Map::saveMap, &map), BRELEASE,
            "SAVE",
            ww - 100, wh - 60, 80, 40);

        menu.createButton(
            std::bind(&StateHandler::setState, this, MENU_CREATE), BRELEASE,
            "MENU",
            20, wh - 60, 80, 40);

        menu.setButtonColorTxt(255, 64, 64, 255);
        menu.createButton(
            std::bind(&Map::resetMap, &map), BRELEASE,
            "CLEAR",
            ww/2 - 40, wh - 60, 80, 40);
    }

    input.inputCreate();

    rend.renderBG();
    rend.renderMap();
    rend.renderButtons();
    rend.show();
}

void
StateHandler::stateRestart()
{
    Game& game = Game::getSingleton();
    game.started = false;
    setState(PLAY);
}
