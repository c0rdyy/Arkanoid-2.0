#include "Game.h"

// Отрисовка Game Over
int ShowEndGameMenu(Game* game)
{
    SDL_Event event;
    int running = 1;
    int restart = 0;

    TTF_Font* font = TTF_OpenFont("fonts/videotype.otf", 24);
    TTF_Font* largeFont = TTF_OpenFont("fonts/videotype.otf", 74);

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color blue = { 82, 255, 255, 255 };
    SDL_Color purple = { 252, 86, 254, 255 };
    SDL_Color yellow = { 255, 255, 0, 255 };

    const char* menuItems[] = { "Restart", "Main Menu", "Exit" };
    int menuItemCount = sizeof(menuItems) / sizeof(menuItems[0]);
    int currentSelection = 0;

    char scoreText[50];
    sprintf_s(scoreText, "YOUR SCORE: %d", game->score);

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
                restart = -1;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    currentSelection = (currentSelection - 1 + menuItemCount) % menuItemCount;
                    break;
                case SDLK_DOWN:
                    currentSelection = (currentSelection + 1) % menuItemCount;
                    break;
                case SDLK_RETURN:
                    if (currentSelection == 0)
                    {
                        restart = 1;
                    }
                    else if (currentSelection == 1)
                    {
                        restart = 0;
                    }
                    else if (currentSelection == 2)
                    {
                        restart = -1;
                    }
                    running = 0;
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        RenderBackground(SCREEN_WIDTH, SCREEN_HEIGHT);

        RenderCenteredText("GAME OVER", largeFont, purple, SCREEN_HEIGHT / 4);
        RenderCenteredText(scoreText, font, yellow, SCREEN_HEIGHT / 2 - 50);


        for (int i = 0; i < menuItemCount; i++)
        {
            RenderCenteredText(menuItems[i], font,
                (i == currentSelection) ? blue : white, SCREEN_HEIGHT / 2 + i * 50);
        }

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    TTF_CloseFont(largeFont);
    return restart;
}

// Отрисовка окна с победой
int ShowVictoryMenu(Game* game)
{
    SDL_Event event;
    int running = 1;
    int result = 0;

    TTF_Font* font = TTF_OpenFont("fonts/videotype.otf", 24);
    TTF_Font* largeFont = TTF_OpenFont("fonts/videotype.otf", 74);

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color blue = { 82, 255, 255, 255 };
    SDL_Color purple = { 252, 86, 254, 255 };
    SDL_Color yellow = { 255, 255, 0, 255 };

    const char* menuItems[] = { "Restart", "Main Menu", "Exit" };
    int menuItemCount = sizeof(menuItems) / sizeof(menuItems[0]);
    int currentSelection = 0;

    char scoreText[50];
    sprintf_s(scoreText, "YOUR SCORE: %d", game->score);

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
                result = -1;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    currentSelection = (currentSelection - 1 + menuItemCount) % menuItemCount;
                    break;
                case SDLK_DOWN:
                    currentSelection = (currentSelection + 1) % menuItemCount;
                    break;
                case SDLK_RETURN:
                    if (currentSelection == 0)
                    {
                        result = 1;
                    }
                    else if (currentSelection == 1)
                    {
                        result = 0;
                    }
                    else if (currentSelection == 2)
                    {
                        result = -1;
                    }
                    running = 0;
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        RenderBackground(SCREEN_WIDTH, SCREEN_HEIGHT);

        RenderCenteredText("VICTORY!", largeFont, purple, SCREEN_HEIGHT / 4);
        RenderCenteredText(scoreText, font, yellow, SCREEN_HEIGHT / 2 - 50);

        for (int i = 0; i < menuItemCount; i++)
        {
            RenderCenteredText(menuItems[i], font,
                (i == currentSelection) ? blue : white, SCREEN_HEIGHT / 2 + i * 50);
        }

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    TTF_CloseFont(largeFont);
    return result;
}

// Отрисовка меню паузы
int ShowPauseMenu(Game* game)
{
    SDL_Event event;
    int running = 1;
    int result = 0;

    TTF_Font* font = TTF_OpenFont("fonts/videotype.otf", 24);
    TTF_Font* largeFont = TTF_OpenFont("fonts/videotype.otf", 74);

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color blue = { 82, 255, 255, 255 };
    SDL_Color purple = { 252, 86, 254, 255 };
    SDL_Color yellow = { 255, 255, 0, 255 };

    const char* menuItems[] = { "Continue", "Restart", "End Game" };
    const int menuItemCount = 3;
    int currentSelection = 0;

    char scoreText[50];
    sprintf_s(scoreText, "SCORE: %d", game->score);

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
                result = -1;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    currentSelection = (currentSelection - 1 + menuItemCount) % menuItemCount;
                    break;
                case SDLK_DOWN:
                    currentSelection = (currentSelection + 1) % menuItemCount;
                    break;
                case SDLK_ESCAPE:
                    result = 1;
                    running = 0;
                    break;
                case SDLK_RETURN:
                    if (currentSelection == 0)
                    {
                        result = 1;
                        running = 0;
                    }
                    else if (currentSelection == 1)
                    {
                        result = 1;
                        running = 0;
                    }
                    else if (currentSelection == 2)
                    {
                        result = 2;
                        running = 0;
                    }
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        RenderBackground(SCREEN_WIDTH, SCREEN_HEIGHT);

        RenderCenteredText("PAUSE MENU", largeFont, purple, SCREEN_HEIGHT / 6);
        RenderCenteredText(scoreText, font, yellow, SCREEN_HEIGHT / 3);

        for (int i = 0; i < menuItemCount; i++)
        {
            RenderCenteredText(menuItems[i], font,
                (i == currentSelection) ? blue : white, SCREEN_HEIGHT / 3 + 50 + i * 40);
        }


        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    TTF_CloseFont(largeFont);
    return result;
}
