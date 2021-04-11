#include <iostream>
#include <thread>
#include <cstdio>
#include <iterator>
#include "snake.h"
#include "render.h"
#include "util.h"

Snake::Snake()
{
    Map& map = Map::getSingleton();
    startX = map.mapX + (map.mapW/2);
    startY = map.mapY + (map.mapH/2);
    initTextures(0);
    addMultipleSegments(2);
    dirX = DIR_NONE;
    dirY = DIR_NONE;
}

void
Snake::addSegment()
{
    Map& map = Map::getSingleton();
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
    map.setTile(segment.rect.x, segment.rect.y, TSNAKE);
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
    Map& map = Map::getSingleton();
    int prevHeadX, prevHeadY, prevTailX, prevTailY;
    if(dirX + dirY)
    {
        auto head = (segments.begin());
        prevHeadX = segments[0].rect.x;
        prevHeadY = segments[0].rect.y;
        prevTailX = (segments.end()-1)->rect.x;
        prevTailY = (segments.end()-1)->rect.y;
        for(auto segment = (segments.end()-1); segment != head; segment--)
        {
            auto prev = (segment-1);
            segment->rect = prev->rect;
            segment->dirX = prev->dirX;
            segment->dirY = prev->dirY;
        }
        segments[0].rect.x += dirX * map.gridSize;
        segments[0].rect.y += dirY * map.gridSize;
        map.setTile(prevHeadX, prevHeadY, TSNAKE);
        map.setTile(prevTailX, prevTailY, TEMPTY);
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
    for(auto pickup = map.pickups.begin(); pickup != map.pickups.end(); pickup++)
    {
        if((pickup->rect.x == segments[0].rect.x) && (pickup->rect.y == segments[0].rect.y))
        {
            int x = (iRandRange(0, map.mapNW - 1) * map.gridSize) + map.mapX; 
            int y = (iRandRange(0, map.mapNH - 1) * map.gridSize) + map.mapY; 
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

// TODO fix positions according to map offset
void
Snake::checkCrash()
{
    Map& map = Map::getSingleton();
    int ww, wh;
    wnd.getSize(ww, wh);

    // Screenwrap.
    if (segments[0].rect.x <= (map.mapX - map.gridSize))
        segments[0].rect.x = (map.mapX + map.mapW - map.gridSize);
    if (segments[0].rect.x >= (map.mapX + map.mapW))
        segments[0].rect.x = map.mapX;

    if (segments[0].rect.y <= (map.mapY - map.gridSize))
        segments[0].rect.y = (map.mapY + map.mapH - map.gridSize);
    if (segments[0].rect.y >= (map.mapY + map.mapH))
        segments[0].rect.y = map.mapY;

    if(map.getTile(segments[0].rect.x, segments[0].rect.y) > TPICKUP)
    {
        state.setState(RESTART);
        map.resetMap();
    }
}
