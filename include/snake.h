#ifndef SNAKE_H
#define SNAKE_H


#include <vector>
#include <thread>
#include "SDL.h"
#include "map.h"
#include "util.h"
#include "stateHandler.h"

enum Dir
{
    DIR_NONE, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT
};
enum SnakeKeyIndex
{
    PKI_UP, PKI_DOWN, PKI_LEFT, PKI_RIGHT, _PKI_N
};

struct SnakeSegment
{
    int row, column;
};

class Snake
{
public:
    std::vector<SnakeSegment> segments;
    int score = 0;
    int stepDelay = 100;
    Dir moveDirection = DIR_NONE;

    Snake();
    void addSegment(int row, int column);
    void setDir(Dir dir);
    void update();
    void initTextures(int snakeNr);

private:
    int player = 0;
    Map& map = Map::getSingleton();
    Window& wnd = Window::getSingleton();
    StateHandler& state = StateHandler::getSingleton();
    SDL_Texture* texHead, *texTail, *texBody, *texCorner;

    void checkCollision();
    void checkCrash();
    void checkPickup();
    void updateSegmentTextures();
};

#endif // SNAKE_H
