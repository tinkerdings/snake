#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#include "inputHandler.h"
#include "render.h"

enum State
{
    PLAY
};
class StateHandler
{
public:
    StateHandler(const StateHandler&) = delete;
    static StateHandler& getSingleton()
    {
        return s_StateHandler;
    }
    void setState(State set);
    void run();

private:
    State current;
    InputHandler& input = InputHandler::getSingleton();
    Render& rend = Render::getSingleton();
    Game& game = Game::getSingleton();

    StateHandler(){}
    static StateHandler s_StateHandler;
    void statePlay();
};

#endif // STATEHANDLER_H
