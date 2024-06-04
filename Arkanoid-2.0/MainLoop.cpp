#include "MainLoop.h"

void MainLoop()
{  
    SDL_Surface* icon = IMG_Load("images/unnamed.png");

    if (icon != NULL)
    {
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }

    int running = 1;
    int inGame = 0;
    int gameMode = 1;

    Game game;
    InitGame(&game);
    Mix_Music* background = NULL;


    while (running)
    {
        if (!inGame)
        {
            running = ShowMainMenu(&inGame, &gameMode);
        }
        else
        {
            int result = GameLoop(&game);

            if (result == -1) 
            {
                running = 0;
            }
            else 
            {
                inGame = 0;
            }
        }
    }
}