#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <cstdlib>
#include <ctime>
#include "window.h"
#include "render.h"
#include "inputHandler.h"
#include "stateHandler.h"
#include "game.h"

#define WINW 800
#define WINH 600

int
main()
{
    std::srand(std::time(nullptr));
    if(SDL_Init(SDL_INIT_EVERYTHING))
    {
        std::cerr << "SDL_Init: " << SDL_GetError() << std::endl;
        return 1;
    }

    Window& wnd = Window::getSingleton();
    Game& game = Game::getSingleton();
    InputHandler& input = InputHandler::getSingleton();
    Render& rend = Render::getSingleton();
    StateHandler& state = StateHandler::getSingleton();
    Map& map = Map::getSingleton();

    // Create window.
    wnd.init("Snake!", WINW, WINH);
    // Create Rendering context.
    rend.init();
    // Setup game, snakes and so forth.
    game.init();
    // Set initial state.
    state.setState(MENU);
    // Load Map.
    map.loadMap();

    rend.setBG(32, 32, 32, 255);
    while(!wnd.shouldQuit())
    {
        state.run();
    }
    
    SDL_Quit();
    return 0;
}
