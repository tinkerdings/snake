#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "window.h"
#include "render.h"
#include "inputHandler.h"

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
    wnd.init("Snake!", 640, 480);

    // Create Rendering context.
    Render& rend = Render::getSingleton();
    rend.init();
    rend.setBG(128, 32, 32);

    // Input handler.
    InputHandler& input = InputHandler::getSingleton();

    while(!wnd.shouldQuit())
    {
        input.handle();

        rend.clear();
//         rend.renderQueue();
        rend.show();
    }
    
    return 0;
}
