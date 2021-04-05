#include <iostream>
#include <thread>
#include <cstdio>
#include <iterator>
#include "snake.h"
#include "render.h"
#include "util.h"

Snake::Snake(bool isPlayer, int x, int y)
{
    startX = x;
    startY = y;
    initTextures(0);
    addSegment();
    addSegment();
    addSegment();
    addSegment();
    addSegment();
    addSegment();
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
        segment.direction = DIR_UP;
        segments.push_back(segment);
    }
    else
    {
        segment.rect = currentTail->rect;
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
            if(prev->tex != texCorner)
                segment->rotation = prev->rotation;
            if((prev->tex != texHead) && (segment->tex != texTail))
                segment->tex = prev->tex;

            if(changedDir)
            {
                (head+1)->tex = texCorner;
                switch((head)->corner)
                {
                case(UPLEFT):
                {
                    (head+1)->flip = SDL_FLIP_NONE;
                    (head+1)->rotation = 0;
                    break;
                }
                case(UPRIGHT):
                {
                    (head+1)->flip = SDL_FLIP_NONE;
                    (head+1)->rotation = 270;
                    break;
                }
                case(DOWNLEFT):
                {
                    (head+1)->flip = SDL_FLIP_NONE;
                    (head+1)->rotation = 90;
                    break;
                }
                case(DOWNRIGHT):
                {
                    (head+1)->flip = SDL_FLIP_NONE;
                    (head+1)->rotation = 180;
                    break;
                }
                case(LEFTUP):
                {
                    (head+1)->flip = SDL_FLIP_NONE;
                    (head+1)->rotation = 180;
                    break;
                }
                case(LEFTDOWN):
                {
                    (head+1)->flip = SDL_FLIP_NONE;
                    (head+1)->rotation = 270;
                    break;
                }
                case(RIGHTUP):
                {
                    (head+1)->flip = SDL_FLIP_NONE;
                    (head+1)->rotation = 90;
                    break;
                }
                case(RIGHTDOWN):
                {
                    (head+1)->flip = SDL_FLIP_NONE;
                    (head+1)->rotation = 0;
                    break;
                }
                }
            }
            else
                head->tex = texBody;
        }
        (segments.end()-1)->tex = texTail;
        head->tex = texHead;
        segments[0].rect.x += dirX * map.gridSize;
        segments[0].rect.y += dirY * map.gridSize;
    }
    std::thread t_collision(&Snake::checkCollision, this);
    t_collision.join();
    changedDir = false;
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
        head->direction = DIR_NONE;
        break;
    }
    case(DIR_UP):
    {
        dirX = 0;
        dirY = -1;
        switch(head->direction)
        {
        case(DIR_LEFT):
        {
            head->corner = UPLEFT;
            break;
        }
        case(DIR_RIGHT):
        {
            head->corner = UPRIGHT;
            break;
        }
        }
        head->direction = DIR_UP;
        head->rotation = 0;
        break;
    }
    case(DIR_DOWN):
    {
        dirX = 0;
        dirY = 1;
        switch(head->direction)
        {
        case(DIR_LEFT):
        {
            head->corner = DOWNLEFT;
            break;
        }
        case(DIR_RIGHT):
        {
            head->corner = DOWNRIGHT;
            break;
        }
        }
        head->direction = DIR_DOWN;
        head->rotation = 180;
        break;
    }
    case(DIR_LEFT):
    {
        dirX = -1;
        dirY = 0;
        switch(head->direction)
        {
        case(DIR_UP):
        {
            head->corner = LEFTUP;
            break;
        }
        case(DIR_DOWN):
        {
            head->corner = LEFTDOWN;
            break;
        }
        }
        head->direction = DIR_LEFT;
        head->rotation = 270;
        break;
    }
    case(DIR_RIGHT):
    {
        dirX = 1;
        dirY = 0;
        switch(head->direction)
        {
        case(DIR_UP):
        {
            head->corner = RIGHTUP;
            break;
        }
        case(DIR_DOWN):
        {
            head->corner = RIGHTDOWN;
            break;
        }
        }
        head->direction = DIR_RIGHT;
        head->rotation = 90;
        break;
    }
    }
    changedDir = true;
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
        texHead     = rend.createTexture("res/snake_head_p1.png");
        texTail     = rend.createTexture("res/snake_tail_p1.png");
        texBody     = rend.createTexture("res/snake_body_p1.png");
        texCorner   = rend.createTexture("res/snake_corner_p1.png");
    }
    else
    {
        texHead     = rend.createTexture("res/snake_head_p2.png");
        texTail     = rend.createTexture("res/snake_tail_p2.png");
        texBody     = rend.createTexture("res/snake_body_p2.png");
        texCorner   = rend.createTexture("res/snake_corner_p2.png");
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
