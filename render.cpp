#include <cstdio>
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "render.h"
#include "game.h"
#include "snake.h"
#include "menu.h"

#define NB_UP       0x01
#define NB_DOWN     0x02
#define NB_LEFT     0x04
#define NB_RIGHT    0x08

Render Render::s_Render;

void
Render::init()
{
    Map& map = Map::getSingleton();
    rend = SDL_CreateRenderer(wnd.getWindow(), -1, SDL_RENDERER_ACCELERATED);
    if(!rend)
    {
        std::cerr << "SDL_CreateRenderer: " << SDL_GetError() << std::endl;
        SDL_Quit();
        std::exit(1);
    }

    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
    int imgInitted = IMG_Init(imgFlags);
    if((imgInitted & imgFlags) != imgFlags)
    {
        std::cerr << "IMG_Init: " << IMG_GetError() << std::endl;
        SDL_Quit();
        std::exit(1);
    }

    if(!TTF_WasInit())
    {
        if(TTF_Init() == -1)
        {
            std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
            SDL_Quit();
            std::exit(1);
        }
        font = TTF_OpenFont("res/PxPlus_IBM_VGA8.ttf", 24);
        fontInput = TTF_OpenFont("res/PxPlus_IBM_VGA8.ttf", 17);
        if(!font || !fontInput)
        {
            std::cerr << "TTF_OpenFont: " << TTF_GetError() << std::endl;
        }
    }

    Menu& menu = Menu::getSingleton();

    logo                = createTexture("res/logo.png");
    bgUI                = createTexture("res/ui-bg-brick.png");
    bgUIFramed          = createTexture("res/ui-bg-brick-framed.png");
    bgUIMapSelect       = createTexture("res/ui-bg-mapselect-brick.png");
    fgUIMapSelect       = createTexture("res/ui-fg-mapselect-brick.png");
    previewFrame        = createTexture("res/tile-preview.png");
    map.bg              = createTexture("res/20x20-bg.png");
    map.texWall         = createTexture("res/20x20-obstacle.png");
    map.texWallCorner   = createTexture("res/20x20-0bstacle-2.png");
    map.texP1Head       = createTexture("res/20x20-head.png");
    map.texP1Body       = createTexture("res/20x20-body.png");
    map.texP1Corner     = createTexture("res/20x20-turn.png");
    map.texP1Tail       = createTexture("res/20x20-tail.png");
    map.texP2Head       = createTexture("res/20x20-head-3.png");
    map.texP2Body       = createTexture("res/20x20-body-3.png");
    map.texP2Corner     = createTexture("res/20x20-turn-3.png");
    map.texP2Tail       = createTexture("res/20x20-tail-3.png");
    pickup              = createTexture("res/20x20-powerup.png");
    initAlphabet();
}

void
Render::freeAllTextures()
{
    for(int i = 0; i < allTextures.size(); i++)
    {
        SDL_DestroyTexture(allTextures[i]);
        allTextures[i] = NULL;
        allTextures.pop_back();
    }
}

void
Render::initAlphabet()
{
    SDL_Color color = {0, 0, 0, 255};
    SDL_Surface *tmp;
    char letter[2] = {0};
    for(char i = 'a'; i <= 'z'; i++)
    {
        sprintf(letter, "%c", i);
        tmp = TTF_RenderText_Solid(fontInput, letter, color);
        if(!tmp)
        {
            std::cerr << "TTF_RenderText_Solid: " << TTF_GetError() << std::endl;
            return;
        }
        alphabet[(int)(i - 'a')] = SDL_CreateTextureFromSurface(rend, tmp);
        if(!alphabet[(int)(i - 'a')])
        {
            std::cerr << "TTF_RenderText_Solid: " << TTF_GetError() << std::endl;
            SDL_FreeSurface(tmp);
            return;
        }
        SDL_FreeSurface(tmp);
    }
}

void
Render::renderLetter(char letter, int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};

    if((letter < 'a') || (letter > 'z'))
    {
        if((letter >= 'A') && (letter <= 'Z'))
        {
            letter ^= 0x20;
        }
        else
        {
            std::cerr << "renderLetter: '" << letter << "' is not a valid letter" << std::endl;
            return;
        }
    }

    SDL_RenderCopy(rend, alphabet[(int)(letter-'a')], NULL, &rect);
}


void
Render::renderMapNameInput()
{
    Map& map = Map::getSingleton();
    Menu& menu = Menu::getSingleton();
    auto btn = (menu.buttons.end() - 1);
    int nLetters = map.editorSaveName.size();

    for(int i = 0; i < nLetters; i++)
    {
        renderLetter(map.editorSaveName[i],
                ((btn->rect.x + btn->rect.w/2) - ((nLetters - i) * 12) + ((nLetters * 12) / 2)),
                (btn->rect.y + btn->rect.h/2),
                10, 20);
    }
}

SDL_Texture*
Render::createTexture(const char* filename)
{
    SDL_Surface *tmp = IMG_Load(filename);
    if(!tmp)
    {
        std::cerr << "IMG_Load: " << IMG_GetError() << std::endl;
        return NULL;
    }
    SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, tmp);
    if(!tex)
    {
        std::cerr << "SDL_CreateTextureFromSurface: " << SDL_GetError() << std::endl;
        return NULL;
    }

    SDL_FreeSurface(tmp);

    allTextures.push_back(tex);
    return tex;
}

void
Render::setClear(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    SDL_SetRenderDrawColor(rend, r, g, b, a);
}

void
Render::clear()
{
    SDL_RenderClear(rend);
}

void
Render::renderBG(SDL_Texture *tex)
{
    int w, h, ww, wh;
    Window& wnd = Window::getSingleton();
    Map& map = Map::getSingleton();

    wnd.getSize(ww, wh);

    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    SDL_Rect out;

    for(int y = 0; y < wh; y+=h)
    {
        for(int x = 0; x < ww; x+=w)
        {
            out.x = x;
            out.y = y;
            out.w = w;
            out.h = h;
            SDL_RenderCopy(rend, tex, NULL, &out);
        }
    }
}

void
Render::renderLogo()
{
    SDL_RenderCopy(rend, logo, NULL, NULL);
}

void
Render::initText(Text *tex, const char* txt, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    SDL_Color color = {r, g, b, a};
    SDL_Surface *surface = TTF_RenderText_Solid(font, txt, color); 
    if(!surface)
    {
        std::cerr << "TTF_RenderText_Solid: " << TTF_GetError() << std::endl;
        return;
    }
    tex->tex = SDL_CreateTextureFromSurface(rend, surface);
    if(!tex->tex)
    {
        std::cerr << "SDL_CreateTextureFromSurface: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }
    allTextures.push_back(tex->tex);
    SDL_QueryTexture(tex->tex, NULL, NULL, &tex->w, &tex->h);
    SDL_FreeSurface(surface);
}

void
Render::renderButtons()
{
    Menu& menu = Menu::getSingleton();
    for(auto button : menu.buttons)
    {
        SDL_RenderCopy(rend, button.tex, NULL, &button.rect);
    }
}

void
Render::renderMapPreviews()
{
    Menu& menu = Menu::getSingleton();
    for(auto preview : menu.mapPreviews)
    {
        SDL_Rect rect = preview.rect;
        rect.y -= menu.mapPreviewScrollOffset;
        SDL_RenderCopy(rend, preview.tex, NULL, &rect);
    }
}

void
Render::renderBorders()
{
    int ww, wh;
    Window& wnd = Window::getSingleton();
    Map& map = Map::getSingleton();

    wnd.getSize(ww, wh);
    setClear(64, 64, 64, 255);
    SDL_Rect top = {0, 0, ww, map.mapY};
    SDL_Rect left = {0, map.mapY, map.mapX, wh - map.mapY};
    SDL_Rect right = {ww - map.mapX, map.mapY, map.mapX, wh - map.mapY};
    SDL_Rect bottom = {map.mapX, wh - map.mapBottom, map.mapW, map.mapBottom};

    SDL_RenderCopy(rend, bgUIFramed, &top, &top);
    SDL_RenderCopy(rend, bgUIFramed, &left, &left);
    SDL_RenderCopy(rend, bgUIFramed, &right, &right);
    SDL_RenderCopy(rend, bgUIFramed, &bottom, &bottom);
}
void
Render::renderScores()
{
    Game& game = Game::getSingleton();
    if(game.snakes.size())
    {
        if(!textP1Name.tex)
            initText(&textP1Name, "P1 SCORE: ", 255, 128, 32, 255);
        char p1Score[100] = {0};
        sprintf(p1Score, "%d", game.snakes[0].score);
    //     if(textP1Score.tex)
    //         SDL_DestroyTexture(textP1Score.tex);
        initText(&textP1Score, p1Score, 255, 255, 0, 255);
        renderText(&textP1Name, 30, 20, textP1Name.w, textP1Name.h);
        renderText(&textP1Score, textP1Name.out.x + textP1Name.w, textP1Name.out.y, textP1Score.w, textP1Name.h);
    }
    if(game.snakes.size() == 2)
    {

    }
}

void
Render::renderText(Text *tex, int x, int y, int w, int h)
{
    tex->out.x = x;
    tex->out.y = y;
    tex->out.w = w;
    tex->out.h = h;
    SDL_RenderCopy(rend, tex->tex, NULL, &tex->out);
}

void
Render::renderMap()
{
    Map& map = Map::getSingleton();
    StateHandler& state = StateHandler::getSingleton();
    SDL_Rect rect;
    SDL_QueryTexture(map.texWall, NULL, NULL, &rect.w, &rect.h);

    for(int j = 0; j < MAPH; j++)
    {
        for(int i = 0; i < MAPW; i++)
        {
            rect.x = (i * map.gridSize) + map.mapX;
            rect.y = (j * map.gridSize) + map.mapY;
            switch(map.map[j][i])
            {
            case(TWALL):
            {
                bool corner = false;
                if((j > 0) && (j < (MAPH-1)) && (i > 0) && (i < (MAPW-1)))
                {
                    if(
                        ((map.map[j-1][i] == TWALL) && (map.map[j][i-1] == TWALL)) ||
                        ((map.map[j-1][i] == TWALL) && (map.map[j][i+1] == TWALL)) ||
                        ((map.map[j+1][i] == TWALL) && (map.map[j][i-1] == TWALL)) ||
                        ((map.map[j+1][i] == TWALL) && (map.map[j][i+1] == TWALL)))
                    {
                        corner = true;
                    }
                }

                if(corner)
                    SDL_RenderCopy(rend, map.texWallCorner, NULL, &rect);
                else
                    SDL_RenderCopy(rend, map.texWall, NULL, &rect);

                break;
            }
            case(TPICKUP):
            {
                SDL_RenderCopy(rend, pickup, NULL, &rect);

                break;
            }
            }

            if(state.current == CREATE)
            {
                switch(map.map[j][i])
                {
                case(TP1HEAD):
                {
                    if((map.getTile(j-1, i) == TP1BODY) || (map.getTile(j-1, i) == TP1TAIL))
                    {
                        SDL_RenderCopyEx(rend, map.texP1Head, NULL, &rect, 180, NULL, SDL_FLIP_NONE);
                    }
                    else if((map.getTile(j+1, i) == TP1BODY) || (map.getTile(j+1, i) == TP1TAIL))
                    {
                        SDL_RenderCopyEx(rend, map.texP1Head, NULL, &rect, 0, NULL, SDL_FLIP_NONE);
                    }
                    else if((map.getTile(j, i-1) == TP1BODY) || (map.getTile(j, i-1) == TP1TAIL))
                    {
                        SDL_RenderCopyEx(rend, map.texP1Head, NULL, &rect, 90, NULL, SDL_FLIP_NONE);
                    }
                    else if((map.getTile(j, i+1) == TP1BODY) || (map.getTile(j, i+1) == TP1TAIL))
                    {
                        SDL_RenderCopyEx(rend, map.texP1Head, NULL, &rect, 270, NULL, SDL_FLIP_NONE);
                    }
                    else
                    {
                        SDL_RenderCopyEx(rend, map.texP1Head, NULL, &rect, 0, NULL, SDL_FLIP_NONE);
                    }

                    break;
                }
                case(TP1BODY):
                {
                    SDL_RenderCopyEx(rend, map.texP1Body, NULL, &rect, 180, NULL, SDL_FLIP_NONE);

                    break;
                }
                case(TP1TAIL):
                {
                    if((map.getTile(j-1, i) == TP1BODY) || (map.getTile(j-1, i) == TP1HEAD))
                    {
                        SDL_RenderCopyEx(rend, map.texP1Tail, NULL, &rect, 0, NULL, SDL_FLIP_NONE);
                    }
                    else if((map.getTile(j+1, i) == TP1BODY) || (map.getTile(j+1, i) == TP1HEAD))
                    {
                        SDL_RenderCopyEx(rend, map.texP1Tail, NULL, &rect, 180, NULL, SDL_FLIP_NONE);
                    }
                    else if((map.getTile(j, i-1) == TP1BODY) || (map.getTile(j, i-1) == TP1HEAD))
                    {
                        SDL_RenderCopyEx(rend, map.texP1Tail, NULL, &rect, 270, NULL, SDL_FLIP_NONE);
                    }
                    else if((map.getTile(j, i+1) == TP1BODY) || (map.getTile(j, i+1) == TP1HEAD))
                    {
                        SDL_RenderCopyEx(rend, map.texP1Tail, NULL, &rect, 90, NULL, SDL_FLIP_NONE);
                    }

                    break;
                }
                case(TP2HEAD):
                {
                    if((map.getTile(j-1, i) == TP2BODY) || (map.getTile(j-1, i) == TP2TAIL))
                    {
                        SDL_RenderCopyEx(rend, map.texP2Head, NULL, &rect, 180, NULL, SDL_FLIP_NONE);
                    }
                    else if((map.getTile(j+1, i) == TP2BODY) || (map.getTile(j+1, i) == TP2TAIL))
                    {
                        SDL_RenderCopyEx(rend, map.texP2Head, NULL, &rect, 0, NULL, SDL_FLIP_NONE);
                    }
                    else if((map.getTile(j, i-1) == TP2BODY) || (map.getTile(j, i-1) == TP2TAIL))
                    {
                        SDL_RenderCopyEx(rend, map.texP2Head, NULL, &rect, 90, NULL, SDL_FLIP_NONE);
                    }
                    else if((map.getTile(j, i+1) == TP2BODY) || (map.getTile(j, i+1) == TP2TAIL))
                    {
                        SDL_RenderCopyEx(rend, map.texP2Head, NULL, &rect, 270, NULL, SDL_FLIP_NONE);
                    }

                    break;
                }
                case(TP2BODY):
                {
                    SDL_RenderCopyEx(rend, map.texP2Body, NULL, &rect, 180, NULL, SDL_FLIP_NONE);

                    break;
                }
                case(TP2TAIL):
                {
                    if((map.getTile(j-1, i) == TP1BODY) || (map.getTile(j-1, i) == TP2HEAD))
                    {
                        SDL_RenderCopyEx(rend, map.texP2Tail, NULL, &rect, 0, NULL, SDL_FLIP_NONE);
                    }
                    else if((map.getTile(j+1, i) == TP2BODY) || (map.getTile(j+1, i) == TP2HEAD))
                    {
                        SDL_RenderCopyEx(rend, map.texP2Tail, NULL, &rect, 180, NULL, SDL_FLIP_NONE);
                    }
                    else if((map.getTile(j, i-1) == TP2BODY) || (map.getTile(j, i-1) == TP2HEAD))
                    {
                        SDL_RenderCopyEx(rend, map.texP2Tail, NULL, &rect, 270, NULL, SDL_FLIP_NONE);
                    }
                    else if((map.getTile(j, i+1) == TP2BODY) || (map.getTile(j, i+1) == TP2HEAD))
                    {
                        SDL_RenderCopyEx(rend, map.texP2Tail, NULL, &rect, 90, NULL, SDL_FLIP_NONE);
                    }

                    break;
                }
                }
            }
        }
    }
}

void
Render::renderSnakes()
{
    Game& game = Game::getSingleton();
    Map& map = Map::getSingleton();
    SDL_Rect rect;
    SDL_QueryTexture(map.texWall, NULL, NULL, &rect.w, &rect.h);
    int playernum = -1;
    SDL_Texture *texHead, *texTail, *texBody, *texCorner;
    for(auto snake = game.snakes.begin(); snake != game.snakes.end(); snake++)
    {
        playernum++;
        switch(playernum)
        {
        case(0):
        {
            texHead = map.texP1Head;
            texTail = map.texP1Tail;
            texBody = map.texP1Body;
            texCorner = map.texP1Corner;

            break;
        }
        case(1):
        {
            texHead = map.texP2Head;
            texTail = map.texP2Tail;
            texBody = map.texP2Body;
            texCorner = map.texP2Corner;

            break;
        }
        }
        for(auto segment = snake->segments.begin(); segment != snake->segments.end(); segment++)
        {
            rect.x = map.mapX + ((segment->column) * map.gridSize);
            rect.y = map.mapY + ((segment->row) * map.gridSize);

            int neighbors = 0;
            if(segment == snake->segments.begin())
            {
                if(((segment+1)->row < segment->row))
                {
                    neighbors |= NB_UP;
                }
                if(((segment+1)->row > segment->row))
                {
                    neighbors |= NB_DOWN;
                }
                if(((segment+1)->column < segment->column))
                {
                    neighbors |= NB_LEFT;
                }
                if(((segment+1)->column > segment->column))
                {
                    neighbors |= NB_RIGHT;
                }
            }
            else if(segment == (snake->segments.end()-1))
            {
                if(((segment-1)->row < segment->row))
                {
                    neighbors |= NB_UP;
                }
                if(((segment-1)->row > segment->row))
                {
                    neighbors |= NB_DOWN;
                }
                if(((segment-1)->column < segment->column))
                {
                    neighbors |= NB_LEFT;
                }
                if(((segment-1)->column > segment->column))
                {
                    neighbors |= NB_RIGHT;
                }
            }
            else
            {
                if(((segment-1)->row < segment->row) || ((segment+1)->row < segment->row))
                {
                    neighbors |= NB_UP;
                }
                if(((segment-1)->row > segment->row) || ((segment+1)->row > segment->row))
                {
                    neighbors |= NB_DOWN;
                }
                if(((segment-1)->column < segment->column) || ((segment+1)->column < segment->column))
                {
                    neighbors |= NB_LEFT;
                }
                if(((segment-1)->column > segment->column) || ((segment+1)->column > segment->column))
                {
                    neighbors |= NB_RIGHT;
                }
            }

            switch(neighbors)
            {
                case(NB_UP):
                {
                    if(segment == (snake->segments.end()-1))
                    {
                        SDL_RenderCopyEx(rend, texTail, NULL, &rect, 0, NULL, SDL_FLIP_NONE);
                    }
                    else
                    {
                        SDL_RenderCopyEx(rend, texHead, NULL, &rect, 180, NULL, SDL_FLIP_NONE);
                    }

                    break;
                }
                case(NB_DOWN):
                {
                    if(segment == (snake->segments.end()-1))
                    {
                        SDL_RenderCopyEx(rend, texTail, NULL, &rect, 180, NULL, SDL_FLIP_NONE);
                    }
                    else
                    {
                        SDL_RenderCopyEx(rend, texHead, NULL, &rect, 0, NULL, SDL_FLIP_NONE);
                    }

                    break;
                }
                case(NB_LEFT):
                {
                    if(segment == (snake->segments.end()-1))
                    {
                        SDL_RenderCopyEx(rend, texTail, NULL, &rect, 270, NULL, SDL_FLIP_NONE);
                    }
                    else
                    {
                        SDL_RenderCopyEx(rend, texHead, NULL, &rect, 90, NULL, SDL_FLIP_NONE);
                    }

                    break;
                }
                case(NB_RIGHT):
                {
                    if(segment == (snake->segments.end()-1))
                    {
                        SDL_RenderCopyEx(rend, texTail, NULL, &rect, 90, NULL, SDL_FLIP_NONE);
                    }
                    else
                    {
                        SDL_RenderCopyEx(rend, texHead, NULL, &rect, 270, NULL, SDL_FLIP_NONE);
                    }

                    break;
                }
                case(NB_UP | NB_DOWN):
                {
                    SDL_RenderCopyEx(rend, texBody, NULL, &rect, 0, NULL, SDL_FLIP_NONE);

                    break;
                }
                case(NB_LEFT | NB_RIGHT):
                {
                    SDL_RenderCopyEx(rend, texBody, NULL, &rect, 90, NULL, SDL_FLIP_NONE);

                    break;
                }
                case(NB_UP | NB_LEFT):
                {
                    SDL_RenderCopyEx(rend, texCorner, NULL, &rect, 90, NULL, SDL_FLIP_NONE);

                    break;
                }
                case(NB_UP | NB_RIGHT):
                {
                    SDL_RenderCopyEx(rend, texCorner, NULL, &rect, 180, NULL, SDL_FLIP_NONE);

                    break;
                }
                case(NB_DOWN | NB_LEFT):
                {
                    SDL_RenderCopyEx(rend, texCorner, NULL, &rect, 0, NULL, SDL_FLIP_NONE);

                    break;
                }
                case(NB_DOWN | NB_RIGHT):
                {
                    SDL_RenderCopyEx(rend, texCorner, NULL, &rect, 270, NULL, SDL_FLIP_NONE);

                    break;
                }
            }
        }
    }
}

void
Render::renderTilePlacementBox()
{
    Map& map = Map::getSingleton();
    InputHandler& input = InputHandler::getSingleton();
    SDL_Rect rect;

    if(((input.mouseX >= map.mapX) && (input.mouseX < map.mapX + map.mapW)) &&
        ((input.mouseY >= map.mapY) && (input.mouseY < map.mapY + map.mapH)))
    {
        if(map.editorValidPlacement)
            setClear(64, 255, 64, 64);
        else
            setClear(255, 64, 64, 64);

        SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
        if((map.editorActiveTile == TP1START) || (map.editorActiveTile == TP2START))
        {
            switch(map.editorRotation)
            {
            case(0):
            {
                rect.x = (input.mouseX / map.gridSize) * map.gridSize;
                rect.y = (input.mouseY / map.gridSize) * map.gridSize;
                rect.w = map.gridSize;
                rect.h = 2*map.gridSize;

                break;
            }
            case(90):
            {
                rect.x = ((input.mouseX / map.gridSize) * map.gridSize) - map.gridSize;
                rect.y = (input.mouseY / map.gridSize) * map.gridSize;
                rect.w = 2*map.gridSize;
                rect.h = map.gridSize;

                break;
            }
            case(180):
            {
                rect.x = (input.mouseX / map.gridSize) * map.gridSize;
                rect.y = ((input.mouseY / map.gridSize) * map.gridSize) - map.gridSize;
                rect.w = map.gridSize;
                rect.h = 2*map.gridSize;

                break;
            }
            case(270):
            {
                rect.x = (input.mouseX / map.gridSize) * map.gridSize;
                rect.y = (input.mouseY / map.gridSize) * map.gridSize;
                rect.w = 2*map.gridSize;
                rect.h = map.gridSize;

                break;
            }
            }
        }
        else
        {
            rect.x = (input.mouseX / map.gridSize) * map.gridSize;
            rect.y = (input.mouseY / map.gridSize) * map.gridSize;
            rect.w = map.gridSize;
            rect.h = map.gridSize;
        }
        SDL_RenderFillRect(rend, &rect);

        SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_NONE);
    }
}

void
Render::renderTilePreview()
{
    Map& map = Map::getSingleton();
    int w, h;
    int previewBoxX = 1;
    int previewBoxY = 81;

    SDL_QueryTexture(previewFrame, NULL, NULL, &w, &h);
    SDL_Rect rectPreviewBox = {previewBoxX, previewBoxY, w, h};
    SDL_RenderCopy(rend, previewFrame, NULL, &rectPreviewBox);

    switch(map.editorActiveTile)
    {
    case(TWALL):
    {
        SDL_QueryTexture(map.texWall, NULL, NULL, &w, &h);
        SDL_Rect rectPreviewTile = {previewBoxX + 21, previewBoxY + 21, w, h};
        SDL_RenderCopy(rend, map.texWall, NULL, &rectPreviewTile);
        break;
    }
    case(TP1START):
    {
        SDL_QueryTexture(map.texP1Head, NULL, NULL, &w, &h);
        SDL_Rect rectPreviewTile = {previewBoxX + 21, previewBoxY + 21, w, h};
        SDL_RenderCopy(rend, map.texP1Head, NULL, &rectPreviewTile);
        break;
    }
    case(TP2START):
    {
        SDL_QueryTexture(map.texP2Head, NULL, NULL, &w, &h);
        SDL_Rect rectPreviewTile = {previewBoxX + 21, previewBoxY + 21, w, h};
        SDL_RenderCopy(rend, map.texP2Head, NULL, &rectPreviewTile);
        break;
    }
    }
}

void
Render::show()
{
    SDL_RenderPresent(rend);
}
