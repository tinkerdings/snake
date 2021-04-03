#include <iostream>
#include <thread>
#include <cstdio>
#include <iterator>
#include "snake.h"
#include "util.h"

Snake::Snake(bool isPlayer, int x, int y)
{
    startX = x;
    startY = y;
    addSegment();
    dirX = DIR_NONE;
    dirY = DIR_NONE;
}

void
Snake::addSegment()
{
    int nSegments;
    SDL_Rect rect;
    SDL_Color color = {32, (unsigned char)iRandRange(128, 255), 32};
    if(!(nSegments = segments.size()))
    {
        rect = {startX, startY + (nSegments * map.gridSize), map.gridSize, map.gridSize};
    }
    else
    {
        auto last = segments.end() - 1;
        rect = last->rect;
    }
    SnakeSegment segment = 
    {
        rect,
        color
    };

    segments.push_back(segment);
}

void
Snake::addMultipleSegments(int amount)
{
    for(int i = 0; i < amount; i++)
        addSegment();
}

Dir
Snake::getDirection()
{
    return direction;
}
void
Snake::setDirection(Dir dir)
{
    switch(dir)
    {
        case(DIR_NONE):
        {
            dirX = 0;
            dirY = 0;
            direction = DIR_NONE;
            break;
        }
        case(DIR_UP):
        {
            dirX = 0;
            dirY = -1;
            direction = DIR_UP;
            break;
        }
        case(DIR_DOWN):
        {
            dirX = 0;
            dirY = 1;
            direction = DIR_DOWN;
            break;
        }
        case(DIR_LEFT):
        {
            dirX = -1;
            dirY = 0;
            direction = DIR_LEFT;
            break;
        }
        case(DIR_RIGHT):
        {
            dirX = 1;
            dirY = 0;
            direction = DIR_RIGHT;
            break;
        }
    }
}

bool
Snake::dirAvailable(Dir dir)
{
    switch (dir)
    {
	case(DIR_UP):
	{
        int newY = segments[0].rect.y - map.gridSize;
        if (segments[1].rect.y == newY)
            return false;
        return true;
        break;
	}
	case(DIR_DOWN):
	{
        int newY = segments[0].rect.y + map.gridSize;
        if (segments[1].rect.y == newY)
            return false;
        return true;
        break;
	}
	case(DIR_LEFT):
	{
        int newX = segments[0].rect.x - map.gridSize;
        if (segments[1].rect.x == newX)
            return false;
        return true;
        break;
	}
	case(DIR_RIGHT):
	{
        int newX = segments[0].rect.x + map.gridSize;
        if (segments[1].rect.x == newX)
            return false;
        return true;
        break;
	}
    }
    return false;
}

void
Snake::update()
{
    if(dirX + dirY)
    {
        for(auto iter = segments.end() - 1; iter != segments.begin(); iter--)
        {
            auto prev = iter-1;
            iter->rect = prev->rect;
        }
        segments[0].rect.x += dirX * map.gridSize;
        segments[0].rect.y += dirY * map.gridSize;
    }
    std::thread t_collision(&Snake::checkCollision, this);
    t_collision.join();
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
    int ww, wh;
    wnd.getSize(ww, wh);
    for(auto pickup = map.pickups.begin(); pickup != map.pickups.end(); pickup++)
    {
        if((pickup->rect.x == segments[0].rect.x) && (pickup->rect.y == segments[0].rect.y))
        {
            int x = iRandRange(0, ww/map.gridSize - 1) * map.gridSize;
            int y = iRandRange(0, wh/map.gridSize - 1) * map.gridSize;
            pickup->setPosition(x, y);
            addSegment();

            if(!(score % intervals))
            {
                if(stepDelay > 30)
					stepDelay -= speedup;
                if (score % 8 && speedup > 2)
                    speedup--;
            }

            score++;
        }
    }
}

void
Snake::checkCrash()
{
    int ww, wh;
    wnd.getSize(ww, wh);

    if (segments[0].rect.x <= -map.gridSize)
        segments[0].rect.x = ww - map.gridSize;
    if (segments[0].rect.x >= ww)
        segments[0].rect.x = 0;

    if (segments[0].rect.y <= -map.gridSize)
        segments[0].rect.y = wh - map.gridSize;
    if (segments[0].rect.y >= wh)
        segments[0].rect.y = 0;

    for(auto segment = segments.begin()+1; segment != segments.end(); segment++)
    {
        if((segment->rect.x == segments[0].rect.x) && (segment->rect.y == segments[0].rect.y))
            state.setState(RESTART);
    }
}
