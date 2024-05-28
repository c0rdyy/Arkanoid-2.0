#include "MainLoop.h"

void MainLoop()
{  
    SDL_Surface* icon = IMG_Load("images/unnamed.png");

    if (icon != NULL)
    {
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }
    else
    {
        SDL_Log("Unable to load icon: %s", IMG_GetError());
    }

    int running = 1;
    int inGame = 0;
    int gameMode = 1;

    while (running)
    {
        if (!inGame)
        {
            running = ShowMainMenu(&inGame, &gameMode);
        }
        else
        {
            running = GameLoop();
            inGame = 0;
        }
    }
}