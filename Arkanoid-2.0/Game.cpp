#include "Game.h"

int brickRows = 5;
int brickCols = 10;
int brickWidth = 60;
int brickHeight = 20;

int totalBrickWidth = brickCols * brickWidth;
int totalSpacing = SCREEN_WIDTH - totalBrickWidth;
int spacingBetweenBricks = totalSpacing / (brickCols + 1);

// Функция для инициализации игры
void InitGame(Game* game)
{
    // Установка начальных значений
    game->gameOver = false;
    game->score = 0;
    game->highScore = 0;
    game->lives = 3;
    game->currentRound = 1;
    game->ballLaunched = false;
    game->powerUpCount = 0;

    // Инициализация параметров платформы
    game->paddle = { 300, 550, 100, 20, 10 };

    // Инициализация параметров мяча
    game->ball = { 350, 530, 10, 10, 0, 0 };

    // Инициализация бонусов
    for (int i = 0; i < MAX_POWERUPS; i++) 
    {
        game->powerUps[i].active = false;
    }

    SDL_Color colors[5] = 
    {
        {255, 0, 0, 255},
        {255, 165, 0, 255},
        {255, 255, 0, 255},
        {0, 255, 0, 255},
        {0, 0, 255, 255}
    };

    for (int i = 0; i < brickRows; i++) 
    {
        for (int j = 0; j < brickCols; j++) 
        {
            int index = i * brickCols + j;
            game->bricks[index] = 
            { 
                j * (brickWidth + spacingBetweenBricks) + spacingBetweenBricks, 
                i * (brickHeight + 5) + 50, 
                brickWidth, 
                brickHeight, 
                colors[i % 5], 
                false 
            };
        }
    }
}

void AddPowerUp(Game* game, int x, int y, int type) 
{
    for (int i = 0; i < MAX_POWERUPS; i++) 
    {
        if (!game->powerUps[i].active) 
        {
            game->powerUps[i].x = x;
            game->powerUps[i].y = y;
            game->powerUps[i].w = 20;  // Ширина бонуса
            game->powerUps[i].h = 20;  // Высота бонуса
            game->powerUps[i].type = type;
            game->powerUps[i].active = true;
            game->powerUpCount++;
            break;
        }
    }
}

// Функция для обновления состояния игры
void UpdateGame(Game* game) 
{
    if (game->gameOver) 
    {
        return;
    }

    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    // Управление платформой
    if (keystates[SDL_SCANCODE_LEFT]) 
    {
        game->paddle.x -= game->paddle.speed;
        if (game->paddle.x < 0) game->paddle.x = 0;
    }
    if (keystates[SDL_SCANCODE_RIGHT]) 
    {
        game->paddle.x += game->paddle.speed;
        if (game->paddle.x + game->paddle.w > SCREEN_WIDTH) game->paddle.x = SCREEN_WIDTH - game->paddle.w;
    }

    // Управление мячом
    if (!game->ballLaunched) 
    {
        game->ball.x = game->paddle.x + game->paddle.w / 2 - game->ball.w / 2;
        game->ball.y = game->paddle.y - game->ball.h - 5;

        if (keystates[SDL_SCANCODE_SPACE]) 
        {
            game->ballLaunched = true;
            game->ball.speedX = 5 + game->currentRound;
            game->ball.speedY = -5 - game->currentRound;
        }
    }
    else 
    {
        game->ball.x += game->ball.speedX;
        game->ball.y += game->ball.speedY;

        if (game->ball.x < 0 || game->ball.x + game->ball.w > SCREEN_WIDTH) 
        {
            game->ball.speedX = -game->ball.speedX;
        }
        if (game->ball.y < 0) 
        {
            game->ball.speedY = -game->ball.speedY;
        }
        if (game->ball.y + game->ball.h > SCREEN_HEIGHT) 
        {
            game->lives--;
            if (game->lives <= 0) 
            {
                game->gameOver = true;
                return;
            }
            else 
            {
                game->ballLaunched = false;
                game->ball.speedX = 0;
                game->ball.speedY = 0;
            }
        }

        SDL_Rect ballRect = { game->ball.x, game->ball.y, game->ball.w, game->ball.h };
        SDL_Rect paddleRect = { game->paddle.x, game->paddle.y, game->paddle.w, game->paddle.h };

        if (SDL_HasIntersection(&ballRect, &paddleRect)) 
        {
            game->ball.speedY = -game->ball.speedY;
            game->ball.y = game->paddle.y - game->ball.h;
        }

        // Обработка столкновений мяча с кирпичами
        for (int i = 0; i < MAX_BRICKS; i++) 
        {
            if (!game->bricks[i].destroyed) 
            {
                SDL_Rect brickRect = { game->bricks[i].x, game->bricks[i].y, game->bricks[i].w, game->bricks[i].h };
                if (SDL_HasIntersection(&ballRect, &brickRect)) 
                {
                    game->bricks[i].destroyed = true;
                    game->ball.speedY = -game->ball.speedY;
                    game->score += 50;

                    if (game->score > game->highScore) 
                    {
                        game->highScore = game->score;
                    }

                    // Генерация бонуса
                    if (rand() % 10 == 0) 
                    {
                        AddPowerUp(game, game->bricks[i].x, game->bricks[i].y, rand() % 3);
                    }
                    break;
                }
            }
        }
    }

    SDL_Rect paddleRectt = { game->paddle.x, game->paddle.y, game->paddle.w, game->paddle.h };

    // Обновление бонусов
    for (int i = 0; i < game->powerUpCount; i++) 
    {
        if (game->powerUps[i].active) 
        {
            game->powerUps[i].y += 5;

            SDL_Rect powerUpRect = { game->powerUps[i].x, 
                game->powerUps[i].y, 
                game->powerUps[i].w, 
                game->powerUps[i].h };

            if (SDL_HasIntersection(&powerUpRect, &paddleRectt)) 
            {
                game->powerUps[i].active = false;
                switch (game->powerUps[i].type) 
                {
                case 0:
                    game->paddle.w += 40;
                    break;
                case 1:
                    game->lives++;
                    break;
                case 2:
                    game->paddle.speed += 3;
                    break;
                }
            }
            if (game->powerUps[i].y > SCREEN_HEIGHT) 
            {
                game->powerUps[i].active = false;
            }
        }
    }
}

void RenderGame(Game* game) 
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    RenderBackground(SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Rect paddleRect = { game->paddle.x, game->paddle.y, game->paddle.w, game->paddle.h };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddleRect);

    SDL_Rect ballRect = { game->ball.x, game->ball.y, game->ball.w, game->ball.h };
    SDL_RenderFillRect(renderer, &ballRect);

    for (int i = 0; i < MAX_BRICKS; i++) 
    {
        if (!game->bricks[i].destroyed) 
        {
            SDL_Rect brickRect = { game->bricks[i].x, game->bricks[i].y, game->bricks[i].w, game->bricks[i].h };
            SDL_SetRenderDrawColor(renderer, game->bricks[i].color.r,
                game->bricks[i].color.g, game->bricks[i].color.b, game->bricks[i].color.a);
            SDL_RenderFillRect(renderer, &brickRect);
        }
    }

    // Отрисовка бонусов
    for (int i = 0; i < game->powerUpCount; i++) 
    {
        if (game->powerUps[i].active) 
        {
            SDL_Rect powerUpRect = { game->powerUps[i].x, 
                game->powerUps[i].y, 
                game->powerUps[i].w, 
                game->powerUps[i].h };
            SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);  // Ярко-розовый цвет для бонусов
            SDL_RenderFillRect(renderer, &powerUpRect);
        }
    }

    TTF_Font* font = TTF_OpenFont("fonts/videotype.otf", 24);
    if (font)
    {
        SDL_Color white = { 255, 255, 255, 255 };

        char scoreText[50];
        snprintf(scoreText, sizeof(scoreText), "SCORE: %d", game->score);
        RenderText(scoreText, 20, 10, font, white);

        char livesText[50];
        snprintf(livesText, sizeof(livesText), "LIVES: %d", game->lives);
        RenderText(livesText, SCREEN_WIDTH - 120, 10, font, white);

        TTF_CloseFont(font);
    }

    SDL_RenderPresent(renderer);
}


int ShowEndGameMenu(int score, int highScore) 
{
    SDL_Event event;
    int running = 1;
    int restart = 0;

    TTF_Font* font = TTF_OpenFont("fonts/videotype.otf", 24);
    TTF_Font* largeFont = TTF_OpenFont("fonts/videotype.otf", 74);

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color blue = { 82, 255, 255, 255 };
    SDL_Color purple = { 252, 86, 254, 255 };

    const char* menuItems[] = { "Restart", "Main Menu" };
    int menuItemCount = sizeof(menuItems) / sizeof(menuItems[0]);
    int currentSelection = 0;
    char scoreText[50];
    char highScoreText[50];

    sprintf_s(scoreText, "YOUR SCORE: %d", score);
    sprintf_s(highScoreText, "HIGH SCORE: %d", highScore);

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
                        running = 0;
                    }
                    else if (currentSelection == 1) 
                    {
                        restart = 0;
                        running = 0;
                    }
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        RenderBackground(SCREEN_WIDTH, SCREEN_HEIGHT);

        RenderCenteredText("GAME OVER", largeFont, purple, SCREEN_HEIGHT / 4);
        RenderCenteredText(scoreText, font, white, SCREEN_HEIGHT / 2 - 50);
        RenderCenteredText(highScoreText, font, white, SCREEN_HEIGHT / 2);


        for (int i = 0; i < menuItemCount; i++) 
        {
            RenderCenteredText(menuItems[i], font, 
                (i == currentSelection) ? blue : white, SCREEN_HEIGHT / 2 + 50 + i * 50);
        }
        
        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    TTF_CloseFont(largeFont);
    return restart;
}

int GameLoop(Game* game)
{
    SDL_Event event;
    int running = 1;

    InitGame(game);

    while (running) 
    {
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) 
            {
                running = 0;
            }
            else if (event.type == SDL_KEYDOWN) 
            {
                if (event.key.keysym.sym == SDLK_ESCAPE) 
                {
                    running = 0;
                }
                else if (event.key.keysym.sym == SDLK_RETURN && game->gameOver) 
                {
                    int result = ShowEndGameMenu(game->score, game->highScore);
                    if (result == 1) 
                    {
                        InitGame(game);
                        game->gameOver = false;
                    }
                    else if (result == 0) 
                    {
                        running = 0;
                    }
                    else if (result == -1) 
                    {
                        running = 0;
                    }
                }
            }
        }

        if (game->gameOver) 
        {
            int result = ShowEndGameMenu(game->score, game->highScore);
            if (result == 1) 
            {
                InitGame(game);
                game->gameOver = false;
            }
            else if (result == 0) 
            {
                running = 0;
            }
            else if (result == -1) 
            {
                running = 0;
            }
        }
        else 
        {
            UpdateGame(game);
            RenderGame(game);
        }

        SDL_Delay(16);
    }

    return running;
}
