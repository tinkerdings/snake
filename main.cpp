#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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
    if(SDL_Init(SDL_INIT_EVERYTHING))
    {
        std::cerr << "SDL_Init: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create window.
    Window& wnd = Window::getSingleton();
    wnd.init("Snake!", WINW, WINH);

    // Setup game, snakes and so forth.
    Game& game = Game::getSingleton();
    game.init();
    InputHandler& input = InputHandler::getSingleton();

    // Create Rendering context.
    Render& rend = Render::getSingleton();
    rend.init();

    StateHandler& state = StateHandler::getSingleton();

    state.setState(PLAY);
    while(!wnd.shouldQuit())
    {
        state.run();
    }
    
    return 0;
}
