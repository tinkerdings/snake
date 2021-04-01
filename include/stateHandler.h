#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#include "inputHandler.h"
#include "render.h"

enum State
{
    PLAY, RESTART
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

    StateHandler(){}
    static StateHandler s_StateHandler;
    void statePlay();
    void stateRestart();
};

#endif // STATEHANDLER_H
