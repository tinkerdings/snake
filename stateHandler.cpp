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
    menu.mapPreviewScrollOffset = 0;
    map.mapFileNames.clear();
    map.getMapFileNames("./maps");
    int posX = 0;
    int posY = 0;

    menu.clearButtons();

    switch(set)
    {
    case(MENU_PLAY_MAPSELECT):
    {
        for(int i = 0; i < map.mapFileNames.size(); i++)
        {
            menu.setButtonColorTxt(255, 64, 64, 255);
            menu.createButton(
                std::bind(&StateHandler::setStateAndLoadMap, this, PLAY, map.mapFileNames[i].c_str()), BRELEASE,
                map.mapFileNames[i].c_str(),
                130 + (posX*200) + (posX*50), 70+(posY*220), 200, 200,
                5, BTPREVIEW);

            if(!((i+1)%3))
            {
                posX = 0;
                posY++;
            }
            else
            {
                posX++;
            }
        }

        break;
    }
    case(MENU_CREATE):
    {
        for(int i = 0; i < map.mapFileNames.size(); i++)
        {
            menu.setButtonColorTxt(255, 64, 64, 255);
            menu.createButton(
                std::bind(&StateHandler::setStateAndLoadMap, this, CREATE, map.mapFileNames[i].c_str()), BRELEASE,
                map.mapFileNames[i].c_str(),
                130 + (posX*200) + (posX*50), 70+(posY*220), 200, 200,
                5, BTPREVIEW);

            if(!((i+1)%3))
            {
                posX = 0;
                posY++;
            }
            else
            {
                posX++;
            }
        }

        break;
    }
    case(CREATE):
    {
        if(map.newMap)
        {
            map.clearMap();
        }

        break;
    }
    }
    current = set;
}
void
StateHandler::setStateAndLoadMap(State set, const char* mapName)
{
    Map& map = Map::getSingleton();
    map.loadMap(mapName);
    map.newMap = false;
    setState(set);
}
void
StateHandler::stateNewMap()
{
    Map& map = Map::getSingleton();
    map.newMap = true;
    setState(CREATE);
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
    case(MENU_PLAY_PLAYERS):
    {
        stateMenuPlayPlayers();
        break;
    }
    case(MENU_PLAY_MAPSELECT):
    {
        stateMenuPlayMapSelect();
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
            std::bind(&StateHandler::setState, this, MENU_PLAY_PLAYERS), BRELEASE,
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
StateHandler::setPlayersAndMapSelect(int players)
{
    Game& game = Game::getSingleton();
    game.players = players;
    setState(MENU_PLAY_MAPSELECT);
}

void
StateHandler::stateMenuPlayPlayers()
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
            std::bind(&StateHandler::setPlayersAndMapSelect, this, 1), BRELEASE,
            "1 PLAYER",
            ww/2 - 200, 240, 400, 100,
            10, BTBUTTON);

        menu.setButtonColorTxt(64, 128, 255, 255);
        menu.createButton(
            std::bind(&StateHandler::setPlayersAndMapSelect, this, 2), BRELEASE,
            "2 PLAYERS",
            ww/2 - 200, 400, 400, 100,
            10, BTBUTTON);

        menu.setButtonColorTxt(255, 64, 64, 255);
        menu.createButton(
            std::bind(&StateHandler::setState, this, MENU), BRELEASE,
            "BACK",
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
StateHandler::stateMenuPlayMapSelect()
{
    Menu& menu = Menu::getSingleton();
    Map& map = Map::getSingleton();
    Render& rend = Render::getSingleton();
    InputHandler& input = InputHandler::getSingleton();
    Window& wnd = Window::getSingleton();
    int ww, wh;
    wnd.getSize(ww, wh);


    if(!menu.buttons.size())
    {
        menu.setButtonColorTxt(255, 255, 64, 255);
        menu.createButton(
            std::bind(&StateHandler::setState, this, MENU_PLAY_PLAYERS), BRELEASE,
            "BACK",
            65, 625, 382, 78,
            10, BTBUTTON);
    }

    timeNow = SDL_GetTicks();
    if((timeNow - timePrev) >= 1000/FPS)
    {
        timePrev = timeNow;

        rend.renderBG(rend.bgUIMapSelect);

        rend.renderMapPreviews();
        rend.renderBG(rend.fgUIMapSelect);
        rend.renderButtons();

        rend.show();
    }

    input.inputMenu();
    if(input.mouseScroll(SCROLLDOWN))
    {
        menu.movePreviews(20);
    }
    if(input.mouseScroll(SCROLLUP))
    {
        menu.movePreviews(-20);
    }
}

void
StateHandler::stateMenuCreate()
{
    Menu& menu = Menu::getSingleton();
    Map& map = Map::getSingleton();
    Render& rend = Render::getSingleton();
    InputHandler& input = InputHandler::getSingleton();
    Window& wnd = Window::getSingleton();
    int ww, wh;
    wnd.getSize(ww, wh);


    if(!menu.buttons.size())
    {
        menu.setButtonColorTxt(255, 255, 64, 255);
        menu.createButton(
            std::bind(&StateHandler::setState, this, MENU), BRELEASE,
            "BACK",
            65, 625, 382, 78,
            10, BTBUTTON);

        menu.setButtonColorTxt(0, 255, 64, 255);
        menu.createButton(
            std::bind(&StateHandler::stateNewMap, this), BRELEASE,
            "NEW",
            513, 625, 382, 78,
            10, BTBUTTON);
    }

    timeNow = SDL_GetTicks();
    if((timeNow - timePrev) >= 1000/FPS)
    {
        timePrev = timeNow;

        rend.renderBG(rend.bgUIMapSelect);

        rend.renderMapPreviews();
        rend.renderBG(rend.fgUIMapSelect);
        rend.renderButtons();

        rend.show();
    }

    input.inputMenu();
    if(input.mouseScroll(SCROLLDOWN))
    {
        menu.movePreviews(20);
    }
    if(input.mouseScroll(SCROLLUP))
    {
        menu.movePreviews(-20);
    }
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
    {
        game.init();
    }

    game.update();

    timeNow = SDL_GetTicks();
    if((timeNow - timePrev) >= 1000/FPS)
    {
        timePrev = timeNow;

        rend.renderBG(map.bg);

        rend.renderMap();
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
            std::bind(&Map::clearMap, &map), BRELEASE,
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
