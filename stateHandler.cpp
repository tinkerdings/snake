#include "stateHandler.h"
#include "game.h"
#include "menu.h"
#include "map.h"

StateHandler StateHandler::s_StateHandler;

void
StateHandler::setState(State set)
{
    Menu& menu = Menu::getSingleton();
    Map& map = Map::getSingleton();
    InputHandler& input = InputHandler::getSingleton();
    menu.clearButtons();
    if(set == CREATE)
    {
        if(!map.mapFileNames.size())
        {
            map.getMapFileNames("./maps");
        }
    }
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

    if(!menu.buttons.size())
    {
        menu.setButtonColorTxt(0, 255, 64, 255);
        menu.createButton(
            std::bind(&StateHandler::setState, this, PLAY), BRELEASE,
            "PLAY",
            ww/2 - 200, 240, 400, 100,
            10, BTBUTTON);

        menu.setButtonColorTxt(64, 128, 255, 255);
        menu.createButton(
            std::bind(&StateHandler::setState, this, MENU_CREATE), BRELEASE,
            "CREATE",
            ww/2 - 200, 400, 400, 100,
            10, BTBUTTON);

        menu.setButtonColorTxt(255, 64, 64, 255);
        menu.createButton(
            std::bind(&Window::quit, &wnd), BRELEASE,
            "QUIT",
            ww/2 - 200, 560, 400, 100,
            10, BTBUTTON);
    }

    timeNow = SDL_GetTicks();
    if((timeNow - timePrev) >= 1000/FPS)
    {
        timePrev = timeNow;

        rend.renderBG(rend.bgUI);
        rend.renderLogo();
        rend.renderButtons();
        rend.show();
    }

    input.inputMenu();
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


    if(!menu.buttons.size())
    {
        menu.setButtonColorTxt(0, 255, 64, 255);

        menu.createButton(
            std::bind(&StateHandler::setState, this, CREATE), BRELEASE,
            "NEW",
            ww/2 - 200, 140, 400, 100,
            10, BTBUTTON);

        menu.setButtonColorTxt(255, 255, 64, 255);
        menu.createButton(
            std::bind(&StateHandler::setState, this, MENU), BRELEASE,
            "BACK",
            ww/2 - 200, 300, 400, 100,
            10, BTBUTTON);

        menu.setButtonColorTxt(64, 64, 255, 255);
        menu.createButton(
            std::bind(&StateHandler::setState, this, CREATE), BRELEASE,
            "ok",
            ww/2 - 200, 460, 200, 200,
            10, BTPREVIEW);
    }

    timeNow = SDL_GetTicks();
    if((timeNow - timePrev) >= 1000/FPS)
    {
        timePrev = timeNow;

        rend.renderBG(rend.bgUI);

        rend.renderButtons();
        rend.renderMapPreviews();

        rend.show();
    }

    input.inputMenu();
}

void
StateHandler::statePlay()
{
    Render& rend = Render::getSingleton();
    Game& game = Game::getSingleton();
    InputHandler& input = InputHandler::getSingleton();
    Menu& menu = Menu::getSingleton();
    Map& map = Map::getSingleton();

    if(!game.started)
		game.init();
    
    game.update();

    timeNow = SDL_GetTicks();
    if((timeNow - timePrev) >= 1000/FPS)
    {
        timePrev = timeNow;

        rend.renderBG(map.bg);

        rend.renderSnakes();
        rend.renderPickups();
        rend.renderBorders();
        rend.renderScores();

        rend.show();
    }

    input.inputPlay();
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
        menu.setButtonColorTxt(255, 255, 64, 255);
        menu.createButton(
            std::bind(&StateHandler::setState, this, MENU_CREATE), BRELEASE,
            "MENU",
            65, 17, 190, 46,
            5, BTBUTTON);

        menu.setButtonColorTxt(255, 64, 64, 255);
        menu.createButton(
            std::bind(&Map::resetMap, &map), BRELEASE,
            "CLEAR",
            ww/2 - 95, 17, 190, 46,
            5, BTBUTTON);

        menu.setButtonColorTxt(0, 255, 64, 255);
        menu.createButton(
            std::bind(&Map::inputMapName, &map), BRELEASE,
            "SAVE",
            705, 17, 190, 46,
            5, BTBUTTON);
    }

    timeNow = SDL_GetTicks();
    input.inputCreate();
    if((timeNow - timePrev) >= 1000/FPS)
    {
        timePrev = timeNow;

        rend.renderBG(map.bg);

        rend.renderBorders();
        rend.renderTilePreview();
        rend.renderMap();
        rend.renderTilePlacementBox();
        rend.renderButtons();
        if(map.savingMap)
        {
            rend.renderMapNameInput();
        }

        rend.show();
    }

}

void
StateHandler::stateRestart()
{
    Game& game = Game::getSingleton();
    game.started = false;
    setState(PLAY);
}
