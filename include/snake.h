#ifndef SNAKE_H
#define SNAKE_H

#include <vector>

struct SnakeSegment
{
    int posX, posY;
};

enum SnakeKeyIndex
{
    PKI_UP, PKI_DOWN, PKI_LEFT, PKI_RIGHT, _PKI_N
};

class Snake
{
public:
    bool isPlayer;
    void snakeSetKey(snakeKeyIndex, int sdlKeyCode);
private:
    std::vector<SnakeSegment> segments;
    int snakeKeyMap[_PKI_N];
};

#endif // SNAKE_H
