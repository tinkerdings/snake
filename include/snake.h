#ifndef SNAKE_H
#define SNAKE_H

#define NB_UP       0x01
#define NB_DOWN     0x02
#define NB_LEFT     0x04
#define NB_RIGHT    0x08

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
    SDL_Rect rect;
    SDL_Texture* tex;
    int rotation = 0;
    int dirX = 0;
    int dirY = 0;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    unsigned char neighbors = 0;
};

class Snake
{
public:
    bool isPlayer;
    std::vector<SnakeSegment> segments;
    int score = 0;
    int stepDelay = 100;

    Snake();
    void snakeSetInputKey(SnakeKeyIndex, int sdlKeyCode);
    void setDirection(Dir dir);
    bool dirAvailable(Dir dir);
    void addSegment();
    void update();
    void initTextures(int snakeNr);
    void updateNeighbors();
    void updateTextures();

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
