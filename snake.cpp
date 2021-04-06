#include <iostream>
#include <thread>
#include <cstdio>
#include <iterator>
#include "snake.h"
#include "render.h"
#include "util.h"

// TODO TRY NEIGHBORS METHOD FOR CORNER TILES.
Snake::Snake(bool isPlayer, int x, int y)
{
    startX = x;
    startY = y;
    initTextures(0);
    addSegment();
    addSegment();
    dirX = DIR_NONE;
    dirY = DIR_NONE;
}

void
Snake::addSegment()
{
    SnakeSegment segment;
    auto currentTail = segments.end()-1;
    if(!segments.size())
    {
        segment.rect = {startX, startY, map.gridSize, map.gridSize};
        segment.tex = texHead;
        segment.dirX = 0;
        segment.dirY = -1;
        segments.push_back(segment);
    }
    else
    {
        segment.rect = {
            currentTail->rect.x - (currentTail->dirX * map.gridSize),
            currentTail->rect.y - (currentTail->dirY * map.gridSize),
            map.gridSize, map.gridSize};
        segment.tex = texTail;
        if(segments.size() > 1)
            currentTail->tex = texBody;
        segments.push_back(segment);
    }
}

void
Snake::addMultipleSegments(int amount)
{
    for(int i = 0; i < amount; i++)
        addSegment();
}

void
Snake::update()
{
    if(dirX + dirY)
    {
        auto head = (segments.begin());
        for(auto segment = (segments.end()-1); segment != head; segment--)
        {
            auto prev = (segment-1);
            segment->rect = prev->rect;
            segment->dirX = prev->dirX;
            segment->dirY = prev->dirY;
        }
        segments[0].rect.x += dirX * map.gridSize;
        segments[0].rect.y += dirY * map.gridSize;
    }
    std::thread t_collision(&Snake::checkCollision, this);
    t_collision.join();
    updateNeighbors();
    updateTextures();
//     changedDir = false;
}

void
Snake::updateNeighbors()
{
    auto segment = segments.begin();
    segment->neighbors = 0;
    if((segment+1)->rect.y < segment->rect.y)
        segment->neighbors |= NB_UP;
    if((segment+1)->rect.y > segment->rect.y)
        segment->neighbors |= NB_DOWN;
    if((segment+1)->rect.x < segment->rect.x)
        segment->neighbors |= NB_LEFT;
    if((segment+1)->rect.x > segment->rect.x)
        segment->neighbors |= NB_RIGHT;
    segment++;
    for(; segment < (segments.end()-1) ; segment++)
    {
        segment->neighbors = 0;
        if(((segment-1)->rect.y < segment->rect.y) || ((segment+1)->rect.y < segment->rect.y))
            segment->neighbors |= NB_UP;
        if(((segment-1)->rect.y > segment->rect.y) || ((segment+1)->rect.y > segment->rect.y))
            segment->neighbors |= NB_DOWN;
        if(((segment-1)->rect.x < segment->rect.x) || ((segment+1)->rect.x < segment->rect.x))
            segment->neighbors |= NB_LEFT;
        if(((segment-1)->rect.x > segment->rect.x) || ((segment+1)->rect.x > segment->rect.x))
            segment->neighbors |= NB_RIGHT;
    }
    segment = (segments.end()-1);
    segment->neighbors = 0;
    if((segment-1)->rect.y < segment->rect.y)
        segment->neighbors |= NB_UP;
    if((segment-1)->rect.y > segment->rect.y)
        segment->neighbors |= NB_DOWN;
    if((segment-1)->rect.x < segment->rect.x)
        segment->neighbors |= NB_LEFT;
    if((segment-1)->rect.x > segment->rect.x)
        segment->neighbors |= NB_RIGHT;
}

void
Snake::updateTextures()
{
    auto segment = segments.begin();
    segment->tex = texHead;
    switch(segment->neighbors)
    {
    case(NB_UP):
    {
        segment->rotation = 180;
        break;
    }
    case(NB_DOWN):
    {
        segment->rotation = 0;
        break;
    }
    case(NB_LEFT):
    {
        segment->rotation = 90;
        break;
    }
    case(NB_RIGHT):
    {
        segment->rotation = 270;
        break;
    }
    }
    segment++;
    for(; segment < (segments.end()-1); segment++)
    {
        if(segment->neighbors == (segment->neighbors & (NB_UP | NB_DOWN)))
        {
            segment->tex = texBody;
            segment->rotation = 0;
            continue;
        }
        if(segment->neighbors == (segment->neighbors & (NB_LEFT | NB_RIGHT)))
        {
            segment->tex = texBody;
            segment->rotation = 90;
            continue;
        }
        if(segment->neighbors == (segment->neighbors & (NB_UP | NB_LEFT)))
        {
            segment->tex = texCorner;
            segment->rotation = 90;
            continue;
        }
        if(segment->neighbors == (segment->neighbors & (NB_UP | NB_RIGHT)))
        {
            segment->tex = texCorner;
            segment->rotation = 180;
            continue;
        }
        if(segment->neighbors == (segment->neighbors & (NB_DOWN | NB_LEFT)))
        {
            segment->tex = texCorner;
            segment->rotation = 0;
            continue;
        }
        if(segment->neighbors == (segment->neighbors & (NB_DOWN | NB_RIGHT)))
        {
            segment->tex = texCorner;
            segment->rotation = 270;
            continue;
        }
    }

    segment = (segments.end()-1);
    segment->tex = texTail;
    switch(segment->neighbors)
    {
    case(NB_UP):
    {
        segment->rotation = 0;
        break;
    }
    case(NB_DOWN):
    {
        segment->rotation = 180;
        break;
    }
    case(NB_LEFT):
    {
        segment->rotation = 270;
        break;
    }
    case(NB_RIGHT):
    {
        segment->rotation = 90;
        break;
    }
    }
}

void
Snake::setDirection(Dir dir)
{
    auto head = segments.begin();
    switch(dir)
    {
    case(DIR_NONE):
    {
        dirX = 0;
        dirY = 0;
        head->dirX = 0;
        head->dirY = 0;
        break;
    }
    case(DIR_UP):
    {
        dirX = 0;
        dirY = -1;
        head->dirX = 0;
        head->dirY = -1;
        break;
    }
    case(DIR_DOWN):
    {
        dirX = 0;
        dirY = 1;
        head->dirX = 0;
        head->dirY = 1;
        break;
    }
    case(DIR_LEFT):
    {
        dirX = -1;
        dirY = 0;
        head->dirX = -1;
        head->dirY = 0;
        break;
    }
    case(DIR_RIGHT):
    {
        dirX = 1;
        dirY = 0;
        head->dirX = 1;
        head->dirY = 0;
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
Snake::initTextures(int snakeNr)
{
    Render& rend = Render::getSingleton();
    if(!snakeNr)
    {
        texHead     = rend.createTexture("res/20x20-head.png");
        texTail     = rend.createTexture("res/20x20-tail.png");
        texBody     = rend.createTexture("res/20x20-body.png");
        texCorner   = rend.createTexture("res/20x20-turn.png");
    }
    else
    {
        texHead     = rend.createTexture("res/20x20-body-3.png");
        texTail     = rend.createTexture("res/20x20-tail-3.png");
        texBody     = rend.createTexture("res/20x20-body-3.png");
        texCorner   = rend.createTexture("res/20x20-turn-3.png");
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
