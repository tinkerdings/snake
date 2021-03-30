#ifndef GAME_H
#define GAME_H

#include <vector>
#include "snake.h"
#include "window.h"

class Game
{
public:
    std::vector<Snake> snakes;

    Game(const Game&) = delete;
    static Game& getSingleton()
    {
        return s_Game;
    }

    void init();
    void update();
private:
    static Game s_Game;
    Window& wnd = Window::getSingleton();
    Uint32 lastTime = 0, currentTime;

    Game(){}
};

#endif // GAME_H
