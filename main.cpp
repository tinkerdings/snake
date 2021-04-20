/* 
 * 
 * main.cpp:
 *
 * Genererer et seed til rand ved nåværende tid, for bruk i å generere 
 * pseudo randome tall der det trengs i prosjektet.
 * Initialiserer SDL2.
 * Oppretter et vindu med gitt navn, bredde og høyde.
 * Initialiserer en rendering context, og initialiserer
 * game classen.
 * Setter også state machine staten til MENU, for å starte spillet i en meny.
 * Og kjører nåværende state i en gameloop til vindu klassen har fått beskjed
 * om å avslutte.
 * Etter game loopen frigjøres alle SDL_Texture's og SDL_Quit() kalles.
 * Nesten alle klassene i dette prosjektet er implementert som en singleton,
 * siden de gjenbrukes rundt omkring i hele prosjektet.
 * Derfor ser vi at her i main opprettes references til flere klasser gjennom
 * å kalle getSingleton() metoden på de gitte klassene.
 *
 */

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

// Vindu dimensjoner.
#define WINW 960
#define WINH 720

// Jeg synes det er lettere å lese funksjonssignaturer med return typen over
// navnet, derfor har jeg skrevet det som
// int
// main() her. Dette gjøres i hele prosjektet.
int
main()
{
    std::srand(std::time(nullptr));
    if(SDL_Init(SDL_INIT_EVERYTHING))
    {
        std::cerr << "SDL_Init: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Får her references til en singleton instance av behøvde klasser.
    Window& wnd         = Window::getSingleton();
    Game& game          = Game::getSingleton();
    InputHandler& input = InputHandler::getSingleton();
    Render& rend        = Render::getSingleton();
    StateHandler& state = StateHandler::getSingleton();
    Map& map            = Map::getSingleton();

    // Oppretter her vindu med egen init metode i Window klassen.
    wnd.init("Snake!", WINW, WINH);
    // Oppretter her en rendering context for SDL.
    rend.init();
    // Setter opp spillebrettet og slangene.
    game.init();
    // Set initial state.
    state.setState(MENU);

    // Fyller vindu bufferen med en farge.
    rend.setClear(32, 32, 32, 255);
    while(!wnd.shouldQuit())
    {
        state.run();
    }
    
    rend.freeAllTextures();
    SDL_Quit();
    return 0;
}
