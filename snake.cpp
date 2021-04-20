#include <iostream>
#include <thread>
#include <cstdio>
#include <iterator>
#include "snake.h"
#include "game.h"
#include "render.h"
#include "util.h"


Snake::Snake()
{
    Map& map = Map::getSingleton();
    Game& game = Game::getSingleton();

    if(!game.snakes.size())
    {
        addSegment(map.P1HeadRow, map.P1HeadColumn);
        addSegment(map.P1TailRow, map.P1TailColumn);
        initTextures(0);
    }
    else
    {
        addSegment(map.P2HeadRow, map.P2HeadColumn);
        addSegment(map.P2TailRow, map.P2TailColumn);
        initTextures(1);
    }
}

void
Snake::addSegment(int row, int column)
{
    SnakeSegment segment;
    segment.row = row;
    segment.column = column;
    segments.push_back(segment);
}

void
Snake::setDir(Dir dir)
{
    switch(dir)
    {
    case(DIR_UP):
    {
        if(segments[1].row >= segments[0].row)
        {
            moveDirection = dir;
            std::cout << "moving up" << std::endl;
        }

        break;
    }
    case(DIR_DOWN):
    {
        if(segments[1].row <= segments[0].row)
        {
            moveDirection = dir;
            std::cout << "moving down" << std::endl;
        }

        break;
    }
    case(DIR_LEFT):
    {
        if(segments[1].column >= segments[0].column)
        {
            moveDirection = dir;
            std::cout << "moving left" << std::endl;
        }


        break;
    }
    case(DIR_RIGHT):
    {
        if(segments[1].column <= segments[0].column)
        {
            moveDirection = dir;
            std::cout << "moving right" << std::endl;
        }

        break;
    }
    }
}
void
Snake::update()
{
    Map& map = Map::getSingleton();

    if(moveDirection != DIR_NONE)
    {
        for(auto segment = segments.end()-1; segment != segments.begin(); segment--)
        {
            map.setTile((segment-1)->row, (segment-1)->column, map.getTile(segment->row, segment->column));
        }
    }
}

void
Snake::initTextures(int snakeNr)
{
    Render& rend = Render::getSingleton();
    switch(snakeNr)
    {
    case(0):
    {
        texHead     = rend.createTexture("res/20x20-head.png");
        texTail     = rend.createTexture("res/20x20-tail.png");
        texBody     = rend.createTexture("res/20x20-body.png");
        texCorner   = rend.createTexture("res/20x20-turn.png");

        break;
    }
    case(1):
    {
        texHead     = rend.createTexture("res/20x20-head-2.png");
        texTail     = rend.createTexture("res/20x20-tail-2.png");
        texBody     = rend.createTexture("res/20x20-body-2.png");
        texCorner   = rend.createTexture("res/20x20-turn-2.png");

        break;
    }
    case(2):
    {
        texHead     = rend.createTexture("res/20x20-head-3.png");
        texTail     = rend.createTexture("res/20x20-tail-3.png");
        texBody     = rend.createTexture("res/20x20-body-3.png");
        texCorner   = rend.createTexture("res/20x20-turn-3.png");

        break;
    }
    }
}

void
Snake::checkCollision()
{
    checkCrash();
    checkPickup();
}

void
Snake::checkPickup()
{

}

// TODO fix positions according to map offset
void
Snake::checkCrash()
{
    Map& map = Map::getSingleton();
    int ww, wh;
    wnd.getSize(ww, wh);
}
