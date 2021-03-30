#include "stateHandler.h"

StateHandler StateHandler::s_StateHandler;

void
StateHandler::setState(State set)
{
    current = set;
}

void
StateHandler::run()
{
    switch(current)
    {
    case(PLAY):
    {
        statePlay();
        break;
    }
    }
}

void
StateHandler::statePlay()
{
    input.inputPlay();

    game.update();

    rend.setBG(128, 32, 32, 255);
    rend.clear();

    rend.renderSnakes();
    rend.renderPickups();
    rend.renderUI();

    rend.show();
}
