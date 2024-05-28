#include "Game.h"

int GameLoop()
{
    SDL_Event event;
    int running = 1;

    while (running) 
    {
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) 
            {
                running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);


        SDL_RenderPresent(renderer);
    }
    return running;
}
