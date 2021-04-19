#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#include "inputHandler.h"
#include "render.h"

enum State
{
    MENU, MENU_CREATE, MENU_PLAY, PLAY, CREATE, RESTART
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
    void setStateAndLoadMap(State set, const char* mapName);
    void stateNewMap();
    void run();

private:
    State current;
    unsigned int timePrev = 0;
    unsigned int timeNow;

    StateHandler(){}
    static StateHandler s_StateHandler;
    void statePlay();
    void stateRestart();
    void stateMenu();
    void stateMenuCreate();
    void stateCreate();
};

#endif // STATEHANDLER_H
