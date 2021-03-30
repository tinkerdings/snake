#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

// Singleton window class.
class Window
{
public:
    Window(const Window&) = delete;

    static Window& getSingleton()
    {
        return s_wnd;
    }

    void init(const char* title, int w, int h);
    bool shouldQuit(){return quitting;}
    void quit(){quitting = true;}
    SDL_Window *getWindow(){return wnd;}
    void getSize(int &w, int &h);

    SDL_Window *wnd;
private:
    Window(){}
    int winW, winH;
    static Window s_wnd;
    bool quitting = false;
};

#endif // WINDOW_H
