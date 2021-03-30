#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL2/SDL.h"

struct SnakeSegment
{
    SDL_Rect rect;
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
    SDL_Color getColor(){return color;}

private:
    int snakeKeyMap[_PKI_N];
};

#endif // SNAKE_H
