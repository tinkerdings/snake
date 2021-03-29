#ifndef GAME_H
#define GAME_H

#include <vector>
#include "snake.h"
#include "window.h"

class Game
{
public:
    Game(const Game&) = delete;

    static Game& getSingleton()
    {
        return s_Game;
    }

    std::vector<Snake> snakes;

private:
    static Game s_Game;
    Window& wnd = Window::getSingleton();

    Game();
};

#endif // GAME_H
