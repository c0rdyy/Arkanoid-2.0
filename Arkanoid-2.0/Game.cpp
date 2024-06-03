#include "Game.h"

// Функция инициализации уровня 1
void InitLevel1(Game* game) 
{
    int brickRows = 8;
    int brickCols = 8;

    int brickWidth = 70;
    int brickHeight = 25;
    int padding = 6;

    // Расчет общей ширины кирпичей
    int totalBricksWidth = brickCols * (brickWidth + padding) - padding;

    // Расчет начальной позиции по x и y
    int startX = (SCREEN_WIDTH - totalBricksWidth) / 2;
    int startY = 60;

    SDL_Color colors[3] =
    {
        {255, 0, 0, 255},    // Красный
        {255, 165, 0, 255},  // Оранжевый
        {255, 255, 0, 255},  // Жёлтый
    };  

    game->brickCount = 0;

    for (int i = 0; i < brickRows; i++) 
    {
        SDL_Color currentColor = colors[rand() % 3];

        for (int j = 0; j < brickCols; j++) 
        {
            if (game->brickCount >= MAX_BRICKS) 
            {
                break;
            }
            game->bricks[game->brickCount] = 
            {
                startX + j * (brickWidth + padding),
                startY + i * (brickHeight + padding),
                brickWidth,
                brickHeight,
                currentColor,
                false
            };
            game->brickCount++;
        }
    }
}

// Функция инициализации уровня 2 (кирпичная стена)
void InitLevel2(Game* game) 
{
    int brickCols = 1;
    int brickRows = 1;

    int brickWidth = 70;
    int brickHeight = 25;

    int padding = 6;

    SDL_Color colors[3] =
    {
        {251, 0, 255, 255},  // Фиолетовый
        {0, 208, 255, 255},    // Голубой
        {255, 0, 136, 255}    // Малиновый
    };

    int totalBricksWidth = brickCols * (brickWidth + padding) - padding;
    int startX = (SCREEN_WIDTH - totalBricksWidth) / 2;
    int startY = 60;

    for (int i = 0; i < brickRows; i++)
    {
        SDL_Color currentColor = colors[rand() % 3];

        int rowStartX = startX + (i % 2 == 0 ? 0 : brickWidth / 2);

        for (int j = 0; j < brickCols; j++)
        {
            if (game->brickCount >= MAX_BRICKS)
            {
                break;
            }
            game->bricks[game->brickCount] =
            {
                rowStartX + j * (brickWidth + padding),
                startY + i * (brickHeight + padding),
                brickWidth,
                brickHeight,
                currentColor,
                false
            };
            game->brickCount++;
        }
    }
}

// Функция инициализации уровня 3
void InitLevel3(Game* game) 
{

}

// Общая функция инициализации уровней
void InitLevel(Game* game) 
{
    // Увеличение скорости мяча
    game->ballSpeed = 6 + game->currentLevel - 1;

    if (game->paddleWidthTimer > 0) 
    {
        game->paddle.w -= 40;
        game->paddleWidthTimer = 0;
    }
    if (game->paddleSpeedTimer > 0) 
    {
        game->paddle.speed -= 3;
        game->paddleSpeedTimer = 0;
    }

    switch (game->currentLevel) 
    {
    case 1:
        InitLevel1(game);
        break;
    case 2:
        InitLevel2(game);
        break;
    case 3:
        InitLevel3(game);
        break;
    default:
        InitLevel1(game);
        break;
    }
}

// Функция инициализации игры
void InitGame(Game* game)
{
    srand(time(NULL));

    // Установка начальных значений
    game->gameOver = false;
    game->score = 0;
    game->highScore = 0;
    game->lives = 3;
    game->ballLaunched = false;
    game->powerUpCount = 0;
    game->ballSpeed = 6;
    game->currentLevel = 1;

    InitLevel(game);

    // Инициализация параметров платформы
    game->paddle = { 350, 550, 100, 20, 10 };

    // Инициализация параметров мяча
    game->ball = { 350, 530, 10, 10, 0, 0 };

    // Инициализация бонусов
    for (int i = 0; i < MAX_POWERUPS; i++) 
    {
        game->powerUps[i].active = false;
    }

}

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
                    if (rand() % 20 == 0) 
                    {
                        AddPowerUp(game, game->bricks[i].x, game->bricks[i].y, rand() % 3);
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
            game->currentLevel = 1;
        }
        ResetBallAndPaddle(game);
        InitLevel(game);
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

// Отрисовка Game Over
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

        if (!game->gameOver) 
        {
            UpdateGame(game);
            RenderGame(game);
        }
        else 
        {
            int result = ShowEndGameMenu(game->score, game->highScore);
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
        }

        SDL_Delay(16);
    }

    return running;
}
