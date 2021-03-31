#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL2/SDL.h"
#include "map.h"
#include "util.h"

struct SnakeSegment
{
    SDL_Rect rect;
    SDL_Color color;
};

enum Dir
{
    DIR_NONE, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT
};
enum SnakeKeyIndex
{
    PKI_UP, PKI_DOWN, PKI_LEFT, PKI_RIGHT, _PKI_N
};

class Snake
{
public:
    bool isPlayer;
    std::vector<SnakeSegment> segments;

    Snake(bool isPlayer, int x, int y);
    void snakeSetInputKey(SnakeKeyIndex, int sdlKeyCode);
    void setDirection(Dir dir);
    void addSegment();
    void addMultipleSegments(int amount);
    void update();

    int dirX, dirY;
private:
    int snakeKeyMap[_PKI_N];
    int startX, startY;
    Map& map = Map::getSingleton();
    Window& wnd = Window::getSingleton();

    bool checkCollision();
    void checkPickup();
};

#endif // SNAKE_H
