#include "stateHandler.h"
#include "game.h"

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
    input.checkQuit();
    switch(current)
    {
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
StateHandler::statePlay()
{
    Render& rend = Render::getSingleton();
    Game& game = Game::getSingleton();
    InputHandler& input = InputHandler::getSingleton();

    if(!game.started)
		game.init();
    
    input.inputPlay();

    game.update();

    rend.setBG(64, 64, 128, 255);
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
