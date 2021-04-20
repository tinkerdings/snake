#ifndef GAME_H
#define GAME_H

#define FPS 60

#include <vector>
#include "snake.h"
#include "window.h"
#include "pickup.h"

class Game
{
public:
    std::vector<Snake> snakes;

    Game(const Game&) = delete;
    static Game& getSingleton()
    {
        return s_Game;
    }

    bool started = false;
    int players = 1;
    void init();
    void restart();
    void update();
private:
    static Game s_Game;
    Window& wnd = Window::getSingleton();
    Map& map = Map::getSingleton();
    Uint32 lastTime = 0, currentTime;

    Game(){}
};

#endif // GAME_H
