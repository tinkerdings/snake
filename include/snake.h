#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL2/SDL.h"

struct SnakeSegment
{
    SDL_Rect rect;
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
    SDL_Color color = {32, 128, 32, 255};
    std::vector<SnakeSegment> segments;
    int segmentSize = 10;

    Snake(bool isPlayer, int x, int y);
    void snakeSetInputKey(SnakeKeyIndex, int sdlKeyCode);
    void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void setDirection(Dir dir);
    void addSegment();
    void addMultipleSegments(int amount);
    SDL_Color getColor(){return color;}
    void update();

    int dirX, dirY;
private:
    int snakeKeyMap[_PKI_N];
    int startX, startY;
};

#endif // SNAKE_H
