#include "InitDeInit.h"
#include "Game_Realization.h"

void Realization()
{
    SDL_Event event;
    int quit = 0;

    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = 1;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        // Рендерим объекты игры здесь

        SDL_RenderPresent(renderer);
    }
}