#include <iostream>
#include <thread>
#include <cstdio>
#include <iterator>
#include "snake.h"
#include "game.h"
#include "pickup.h"
#include "render.h"
#include "util.h"


Snake::Snake()
{
    Map& map = Map::getSingleton();
    Game& game = Game::getSingleton();

    if(!game.snakes.size())
    {
        player = 0;
        addSegment(map.P1HeadRow, map.P1HeadColumn);
        addSegment(map.P1TailRow, map.P1TailColumn);
        initTextures(player);
    }
    else
    {
        player = 1;
        addSegment(map.P2HeadRow, map.P2HeadColumn);
        addSegment(map.P2TailRow, map.P2TailColumn);
        initTextures(player);
    }
}

void
Snake::addSegment(int row, int column)
{
    Map& map = Map::getSingleton();
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
        }

        break;
    }
    case(DIR_DOWN):
    {
        if(segments[1].row <= segments[0].row)
        {
            moveDirection = dir;
        }

        break;
    }
    case(DIR_LEFT):
    {
        if(segments[1].column >= segments[0].column)
        {
            moveDirection = dir;
        }


        break;
    }
    case(DIR_RIGHT):
    {
        if(segments[1].column <= segments[0].column)
        {
            moveDirection = dir;
        }

        break;
    }
    }
}
void
Snake::update()
{
    Game& game = Game::getSingleton();
    Map& map = Map::getSingleton();
    StateHandler& state = StateHandler::getSingleton();

    TileType snakeHeadTile = map.getTile(segments[0].row, segments[0].column);
    if(moveDirection != DIR_NONE)
    {
        auto segment = segments.end()-1;
        auto tailRow = (segments.end()-1)->row;
        auto tailColumn = (segments.end()-1)->column;
        for(; segment != segments.begin(); segment--)
        {
            map.setTile((segment-1)->row, (segment-1)->column, map.getTile(segment->row, segment->column));
            segment->row = (segment-1)->row;
            segment->column = (segment-1)->column;
        }
        map.setTile(tailRow, tailColumn, TEMPTY);

        int newRow = segment->row;
        int newColumn = segment->column;

        switch(moveDirection)
        {
        case(DIR_UP):
        {
            newRow = (segment->row)-1;
            if(!(newRow >= 0))
                newRow = MAPH-1;

            break;
        }
        case(DIR_DOWN):
        {
            newRow = (segment->row)+1;
            if(!(newRow <= MAPH-1))
                newRow = 0;

            break;
        }
        case(DIR_LEFT):
        {
            newColumn = (segment->column)-1;
            if(!(newColumn >= 0))
                newColumn = MAPW-1;

            break;
        }
        case(DIR_RIGHT):
        {
            newColumn = (segment->column)+1;
            if(!(newColumn <= (MAPW-1)))
                newColumn = 0;
            break;
        }
        }

        switch(map.getTile(newRow, newColumn))
        {
        case(TEMPTY):
        {
            map.setTile(newRow, newColumn, snakeHeadTile);
            segment->row = newRow;
            segment->column = newColumn;

            break;
        }
        case(TPICKUP):
        {
            map.setTile(newRow, newColumn, snakeHeadTile);
            segment->row = newRow;
            segment->column = newColumn;
            spawnPickup();
            score++;
            if(score % 2)
            {
                stepDelay -= 4;
                if(stepDelay < 40)
                    stepDelay = 40;
            }
            int beforeTailRow = (segments.end()-2)->row;
            int beforeTailColumn = (segments.end()-2)->column;
            
            if(beforeTailRow < tailRow)
            {
                addSegment(tailRow, tailColumn);
            }
            else if(beforeTailRow > tailRow)
            {
                addSegment(tailRow, tailColumn);
            }
            else if(beforeTailColumn < tailColumn)
            {
                addSegment(tailRow, tailColumn);
            }
            else if(beforeTailColumn > tailRow)
            {
                addSegment(tailRow, tailColumn);
            }

            break;
        }
        case(TP1HEAD):
        case(TP1TAIL):
        case(TP1BODY):
        case(TP2HEAD):
        case(TP2TAIL):
        case(TP2BODY):
        case(TWALL):
        {
            game.started = false;
            state.setStateAndLoadMap(PLAY, map.currentMapName.c_str());

            break;
        }
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
