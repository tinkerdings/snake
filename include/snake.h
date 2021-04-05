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
enum Corner
{
    UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT, LEFTUP, LEFTDOWN, RIGHTUP, RIGHTDOWN
};
enum SnakeKeyIndex
{
    PKI_UP, PKI_DOWN, PKI_LEFT, PKI_RIGHT, _PKI_N
};

struct SnakeSegment
{
    SDL_Rect rect;
    SDL_Texture* tex;
    int rotation = 0;
    Dir direction = DIR_NONE;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    Corner corner;
    unsigned int neighbors;
};

class Snake
{
public:
    bool isPlayer;
    std::vector<SnakeSegment> segments;
    int score = 0;
    int stepDelay = 500;

    Snake(bool isPlayer, int x, int y);
    void snakeSetInputKey(SnakeKeyIndex, int sdlKeyCode);
    void setDirection(Dir dir);
    bool dirAvailable(Dir dir);
    void addSegment();
    void addMultipleSegments(int amount);
    void update();
    void initTextures(int snakeNr);

    int dirX, dirY;
private:
    int snakeKeyMap[_PKI_N];
    int startX, startY;
    int intervals = 4;
    int speedup = 5;
    Map& map = Map::getSingleton();
    Window& wnd = Window::getSingleton();
    StateHandler& state = StateHandler::getSingleton();
    SDL_Texture* texHead, *texTail, *texBody, *texCorner;
    bool changedDir = false;

    void checkCollision();
    void checkCrash();
    void checkPickup();
    void updateSegmentTextures();
};

#endif // SNAKE_H
