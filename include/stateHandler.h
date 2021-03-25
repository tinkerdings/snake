#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#define STATE_MENU_MAIN         0x01
#define STATE_MENU_LVL          0x02
#define STATE_MENU_SETTINGS     0x04
#define STATE_GAME_PLAY         0x08
#define STATE_GAME_PAUSE        0x10
#define STATE_GAME_LOSE         0x20
#define STATE_GAME_HIGHSCORE    0x40

typedef unsigned char STATE;

class StateHandler
{
public:
    StateHandler(const StateHandler&) = delete;

    static StateHandler& getSingleton()
    {
        return s_StateHandler;
    }

    STATE current(){return state;};
    void init(STATE setTo){state &= 0; state |= setTo;};
    void add(STATE add){state |= add;};
    void remove(STATE remove){state &= ~remove;};

private:
    StateHandler(){}
    static StateHandler s_StateHandler;

    STATE state = 0;
};

#endif // STATEHANDLER_H
