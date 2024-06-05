#include "Game.h"

// Функция для обновления рекордов
void UpdateHighScores(Game* game) 
{
    int score = game->score;
    for (int i = 0; i < 5; i++) 
    {
        if (score > game->scores[i]) 
        {
            for (int j = 4; j > i; j--) 
            {
                game->scores[j] = game->scores[j - 1];
            }
            game->scores[i] = score;
            break;
        }
    }
    SaveHighScores(game->scores, 5);
}

// Функция
bool AllBricksDestroyed(Game* game) 
{
    for (int i = 0; i < game->brickCount; i++) 
    {
        if (!game->bricks[i].destroyed) 
        {
            return false;
        }
    }
    return true;
}

// Функция для добавления бонусов
void AddPowerUp(Game* game, int x, int y, int type) 
{
    for (int i = 0; i < MAX_POWERUPS; i++) 
    {
        if (!game->powerUps[i].active) 
        {
            game->powerUps[i].x = x;
            game->powerUps[i].y = y;
            game->powerUps[i].w = 20;
            game->powerUps[i].h = 20;
            game->powerUps[i].type = type;
            game->powerUps[i].active = true;
            game->powerUpCount++;
            break;
        }
    }
}

// Функция для сброса позиции мяча и платформы
void ResetBallAndPaddle(Game* game)
{
    // Сброс мяча
    game->ball.x = game->paddle.x + game->paddle.w / 2 - game->ball.w / 2;
    game->ball.y = game->paddle.y - game->ball.h - 5;
    game->ball.speedX = 0;
    game->ball.speedY = 0;
    game->ballLaunched = false;

    // Сброс платформы
    game->paddle.x = 350;
    game->paddle.y = 550;
    game->paddle.w = 100;
    game->paddle.h = 20;
    game->paddle.speed = 10;
}

// Функция для подсчета оставшихся кирпичей
int CountRemainingBricks(Game* game) 
{
    int count = 0;
    for (int i = 0; i < MAX_BRICKS; i++) 
    {
        if (!game->bricks[i].destroyed) 
        {
            count++;
        }
    }
    return count;
}

// Функция для обновления состояния игры
void UpdateGame(Game* game) 
{
    if (game->gameOver) 
    {
        return;
    }

    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    if (CountRemainingBricks(game) == 0)
    {
        game->currentLevel++;
        if (game->currentLevel > MAX_LEVELS)
        {
            game->victory = true;
        }
        else
        {
            ResetBallAndPaddle(game);
            InitLevel(game);
        }
        return;
    }

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

            int directionX = (rand() % 2) * 2 - 1;
            game->ball.speedX = directionX * 6;
            game->ball.speedY = -6;
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
        
        // Падение мяча за нижнюю границу экрана
        if (game->ball.y + game->ball.h > SCREEN_HEIGHT) 
        {
            game->lives--;
            if (game->lives <= 0) 
            {
                game->gameOver = true;
            }
            else 
            {
                ResetBallAndPaddle(game);
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
                SDL_Rect brickRect = 
                { 
                    game->bricks[i].x, 
                    game->bricks[i].y, 
                    game->bricks[i].w, 
                    game->bricks[i].h 
                };

                if (SDL_HasIntersection(&ballRect, &brickRect))
                {
                    // Определение стороны столкновения
                    int ballCenterX = game->ball.x + game->ball.w / 2;
                    int ballCenterY = game->ball.y + game->ball.h / 2;
                    int brickCenterX = game->bricks[i].x + game->bricks[i].w / 2;
                    int brickCenterY = game->bricks[i].y + game->bricks[i].h / 2;

                    int dx = ballCenterX - brickCenterX;
                    int dy = ballCenterY - brickCenterY;

                    if (abs(dx) > abs(dy))
                    {
                        // Столкновение с левой или правой стороной
                        game->ball.speedX = -game->ball.speedX;
                    }
                    else
                    {
                        // Столкновение с верхней или нижней стороной
                        game->ball.speedY = -game->ball.speedY;
                    }

                    game->bricks[i].hitPoints--;
                    if (game->bricks[i].hitPoints <= 0)
                    {
                        game->bricks[i].destroyed = true;
                        game->score += 50;

                        if (game->score > game->highScore)
                        {
                            game->highScore = game->score;
                        }

                        // Генерация бонуса
                        if (rand() % 20 == 0)
                        {
                            AddPowerUp(game, game->bricks[i].x, game->bricks[i].y, rand() % 3);
                        }
                    }
                    break;
                }
                
            }
        }
    }

    SDL_Rect paddleRectt = { game->paddle.x, game->paddle.y, game->paddle.w, game->paddle.h };

    // Сброс бонусов
    if (game->paddleWidthTimer > 0) 
    {
        game->paddleWidthTimer--;
        if (game->paddleWidthTimer == 0) 
        {
            game->paddle.w -= 40;
        }
    }
    if (game->paddleSpeedTimer > 0) 
    {
        game->paddleSpeedTimer--;
        if (game->paddleSpeedTimer == 0) 
        {
            game->paddle.speed -= 3;
        }
    }

    // Обновление бонусов
    for (int i = 0; i < game->powerUpCount; i++)
    {
        if (game->powerUps[i].active)
        {
            game->powerUps[i].y += 5;  // Падение бонуса

            SDL_Rect powerUpRect = 
            { 
                game->powerUps[i].x, 
                game->powerUps[i].y, 
                game->powerUps[i].w, 
                game->powerUps[i].h 
            };

            SDL_Rect paddleRect = 
            { 
                game->paddle.x, 
                game->paddle.y, 
                game->paddle.w, 
                game->paddle.h 
            };

            if (SDL_HasIntersection(&powerUpRect, &paddleRect))
            {
                game->powerUps[i].active = false;

                // Активация бонуса
                switch (game->powerUps[i].type)
                {
                case 0:  // Увеличение ширины
                    game->paddle.w += 40;
                    game->paddleWidthTimer = 400;
                    break;
                case 1:  // Дополнительная жизнь
                    game->lives++;
                    break;
                case 2:  // Увеличение скорости
                    game->paddle.speed += 3;
                    game->paddleSpeedTimer = 400;
                    break;
                }
            }
            if (game->powerUps[i].y > SCREEN_HEIGHT)
            {
                game->powerUps[i].active = false;
            }
        }
    }

    if (AllBricksDestroyed(game))
    {
        game->currentLevel++;
        if (game->currentLevel > MAX_LEVELS)
        {
            game->victory = true;
        }
        else
        {
            ResetBallAndPaddle(game);
            InitLevel(game);
        }  
    }
}

// Функция для отрисовки игры
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
            SDL_Rect brickRect = 
            { 
                game->bricks[i].x, 
                game->bricks[i].y, 
                game->bricks[i].w, 
                game->bricks[i].h 
            };
            SDL_SetRenderDrawColor(renderer, game->bricks[i].color.r,
                game->bricks[i].color.g, game->bricks[i].color.b, game->bricks[i].color.a);
            SDL_RenderFillRect(renderer, &brickRect);
        }
    }

    // Цвета для бонусов
    SDL_Color bonusColors[] =
    {
        {255, 0, 0, 255}, // Красный
        {0, 255, 0, 255}, // Зеленый
        {0, 0, 255, 255} // Синий
    };

    // Отрисовка бонусов
    for (int i = 0; i < game->powerUpCount; i++) 
    {
        if (game->powerUps[i].active) 
        {
            SDL_Rect powerUpRect = 
            { 
                game->powerUps[i].x, 
                game->powerUps[i].y, 
                game->powerUps[i].w, 
                game->powerUps[i].h };

            SDL_Color powerUpColor = bonusColors[game->powerUps[i].type];
            
            SDL_SetRenderDrawColor(renderer, powerUpColor.r, powerUpColor.g, powerUpColor.b, powerUpColor.a);
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

// Основной цикл игры
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
                return -1;
            }
            else if (event.type == SDL_KEYDOWN) 
            {
                if (event.key.keysym.sym == SDLK_ESCAPE) 
                {
                    int result = ShowPauseMenu(game);
                    if (result == 1)
                    {
                        InitGame(game);
                    }
                    else if (result == 2)
                    {
                        running = 0;
                    }
                    else if (result == -1)
                    {
                        return -1;
                    }
                }
                else if (event.key.keysym.sym == SDLK_RETURN && game->gameOver) 
                {
                    int result = ShowEndGameMenu(game);
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
                else if (event.key.keysym.sym == SDLK_RETURN && game->victory)
                {
                    int result = ShowVictoryMenu(game);
                    if (result == 1)
                    {
                        InitGame(game);
                        game->victory = false;
                    }
                    else if (result == 0 || result == -1)
                    {
                        running = 0;
                    }
                }
            }
        }

        if (!game->gameOver && !game->victory) 
        {
            UpdateGame(game);
            RenderGame(game);
        }
        else if (game->gameOver)
        {
            UpdateHighScores(game);
            int result = ShowEndGameMenu(game);
            if (result == 1) 
            {
                game->currentLevel = 1;
                InitGame(game);
                game->gameOver = false;
            }
            else if (result == 0) 
            {
                running = 0;
            }
            else if (result == -1)
            {
                return -1;
            }
        }
        else if (game->victory)
        {
            UpdateHighScores(game);
            int result = ShowVictoryMenu(game);
            if (result == 1)
            {
                game->currentLevel = 1;
                InitGame(game);
                game->victory = false;
            }
            else if (result == 0)
            {
                running = 0;
            }
            else if (result == -1)
            {
                return -1;
            }
        }

        SDL_Delay(16);
    }

    return running;
}
