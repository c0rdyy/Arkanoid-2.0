#include "Game.h"

Paddle paddle;
Ball ball;
Brick bricks[MAX_BRICKS];
PowerUp powerUp;

int brickRows = 5;
int brickCols = 10;
int brickWidth = 60;
int brickHeight = 20;

int totalBrickWidth = brickCols * brickWidth;
int totalSpacing = SCREEN_WIDTH - totalBrickWidth;
int spacingBetweenBricks = totalSpacing / (brickCols + 1);

int score = 0;
int currentRound = 1;
int highScore = 0;
int lives = 3;

bool ballLaunched = false;
bool gameOver = false;

void InitGame() 
{
    paddle = { 300, 550, 100, 20, 10 };
    ball = { 350, 530, 10, 10, 0, 0 };
    ballLaunched = false;
    powerUp = { 0, 0, 20, 20, 0, false };
    gameOver = false;

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
            bricks[index] = { j * (brickWidth + 
                spacingBetweenBricks) + spacingBetweenBricks, 
                i * (brickHeight + 5) + 50, brickWidth, brickHeight, 
                colors[i % 5], false };
        }
    }
}

void UpdateGame() 
{
    if (gameOver) 
    {
        return;
    }

    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    if (keystates[SDL_SCANCODE_LEFT]) 
    {
        paddle.x -= paddle.speed;
        if (paddle.x < 0) paddle.x = 0;
    }

    if (keystates[SDL_SCANCODE_RIGHT]) 
    {
        paddle.x += paddle.speed;
        if (paddle.x + paddle.w > SCREEN_WIDTH) paddle.x = SCREEN_WIDTH - paddle.w;
    }

    if (!ballLaunched) 
    {
        ball.x = paddle.x + paddle.w / 2 - ball.w / 2;
        ball.y = paddle.y - ball.h - 5;

        if (keystates[SDL_SCANCODE_SPACE]) 
        {
            ballLaunched = true;
            ball.speedX = 5 + currentRound;
            ball.speedY = -5 - currentRound;
        }
    }
    else 
    {
        ball.x += ball.speedX;
        ball.y += ball.speedY;

        if (ball.x < 0 || ball.x + ball.w > SCREEN_WIDTH) 
        {
            ball.speedX = -ball.speedX;
        }
        if (ball.y < 0) 
        {
            ball.speedY = -ball.speedY;
        }
        if (ball.y + ball.h > SCREEN_HEIGHT) 
        {
            lives--;
            if (lives <= 0) 
            {
                gameOver = true;
                return;
            }
            else 
            {
                ballLaunched = false;
                ball.speedX = 0;
                ball.speedY = 0;
            }
        }

        SDL_Rect ballRect = { ball.x, ball.y, ball.w, ball.h };
        SDL_Rect paddleRect = { paddle.x, paddle.y, paddle.w, paddle.h };

        if (SDL_HasIntersection(&ballRect, &paddleRect)) 
        {
            ball.speedY = -ball.speedY;
            ball.y = paddle.y - ball.h;
        }

        for (int i = 0; i < brickRows * brickCols; i++) 
        {
            Brick& brick = bricks[i];
            SDL_Rect brickRect = { brick.x, brick.y, brick.w, brick.h };
            if (!brick.destroyed && SDL_HasIntersection(&ballRect, &brickRect)) 
            {
                brick.destroyed = true;
                ball.speedY = -ball.speedY;
                score += 100;

                if (score > highScore) 
                {
                    highScore = score;
                }

                // Бонус
                if (rand() % 10 == 0) 
                {
                    powerUp.x = brick.x;
                    powerUp.y = brick.y;
                    powerUp.type = rand() % 3;
                    powerUp.active = true;
                }
                break;
            }
        }
    }

    // Обновление бонуса
    if (powerUp.active) 
    {
        powerUp.y += 5;
        SDL_Rect powerUpRect = { powerUp.x, powerUp.y, powerUp.w, powerUp.h };
        SDL_Rect paddleRect = { paddle.x, paddle.y, paddle.w, paddle.h };

        if (SDL_HasIntersection(&powerUpRect, &paddleRect)) 
        {
            powerUp.active = false;
            switch (powerUp.type) 
            {
            case 0: // Увеличение платформы
                paddle.w += 20;
                break;
            case 1: // Дополнительная жизнь
                lives++;
                break;
            case 2: // Увеличение скорости
                paddle.speed += 2;
                break;
            }
        }

        if (powerUp.y > SCREEN_HEIGHT) 
        {
            powerUp.active = false;
        }
    }
}

void RenderGame()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    RenderBackground(SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Rect paddleRect = { paddle.x, paddle.y, paddle.w, paddle.h };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddleRect);

    SDL_Rect ballRect = { ball.x, ball.y, ball.w, ball.h };
    SDL_RenderFillRect(renderer, &ballRect);

    for (int i = 0; i < brickRows * brickCols; i++)
    {
        if (!bricks[i].destroyed)
        {
            SDL_Rect brickRect = { bricks[i].x, bricks[i].y, bricks[i].w, bricks[i].h };
            SDL_SetRenderDrawColor(renderer, bricks[i].color.r,
                bricks[i].color.g, bricks[i].color.b, bricks[i].color.a);
            SDL_RenderFillRect(renderer, &brickRect);
        }
    }

    // Отрисовка бонуса
    if (powerUp.active)
    {
        SDL_Rect powerUpRect = { powerUp.x, powerUp.y, powerUp.w, powerUp.h };
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderFillRect(renderer, &powerUpRect);
    }

    TTF_Font* font = TTF_OpenFont("fonts/videotype.otf", 24);
    if (font)
    {
        SDL_Color white = { 255, 255, 255, 255 };

        char scoreText[50];
        snprintf(scoreText, sizeof(scoreText), "SCORE: %d", score);
        RenderText(scoreText, 20, 10, font, white);

        char livesText[50];
        snprintf(livesText, sizeof(livesText), "LIVES: %d", lives);
        RenderText(livesText, SCREEN_WIDTH - 120, 10, font, white);

        TTF_CloseFont(font);
    }

    SDL_RenderPresent(renderer);
}

int ShowEndGameMenu(int score) 
{
    SDL_Event event;
    int running = 1;
    int restart = 0;

    TTF_Font* font = TTF_OpenFont("fonts/videotype.otf", 24);
    TTF_Font* largeFont = TTF_OpenFont("fonts/videotype.otf", 74);


    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color blue = { 82, 255, 255, 255 };
    SDL_Color purple = { 252, 86, 254, 255 };

    const char* menuItems[] = {
        "Restart",
        "Main Menu"
    };

    int menuItemCount = sizeof(menuItems) / sizeof(menuItems[0]);
    int currentSelection = 0;

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
                        restart = 0; // Возврат в главное меню
                        running = 0;
                    }
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        RenderBackground(SCREEN_WIDTH, SCREEN_HEIGHT);

        char gameOverText[50];
        snprintf(gameOverText, sizeof(gameOverText), "GAME OVER");
        RenderText(gameOverText, SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 150, largeFont, purple);

        char scoreText[50];
        snprintf(scoreText, sizeof(scoreText), "SCORE: %d", score);
        RenderText(scoreText, SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 50, font, white);

        char highScoreText[50];
        snprintf(highScoreText, sizeof(highScoreText), "HIGH SCORE: %d", highScore);
        RenderText(highScoreText, SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, font, white);

        int y = SCREEN_HEIGHT / 2 + 50;
        for (int i = 0; i < menuItemCount; i++) 
        {
            int width, height;
            TTF_SizeText(font, menuItems[i], &width, &height);
            RenderText(menuItems[i], SCREEN_WIDTH / 2 - width / 2, 
                y, font, (i == currentSelection) ? blue : white);
            y += height + 10;
        }

        RenderButtonFrame(SCREEN_WIDTH / 2 - 100, 
            SCREEN_HEIGHT / 2 + 40, 200, 100, purple);
        RenderWindowFrame(purple, SCREEN_WIDTH, SCREEN_HEIGHT);

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    TTF_CloseFont(largeFont);
    return restart;
}

void RenderEndScreen() 
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    RenderBackground(SCREEN_WIDTH, SCREEN_HEIGHT);

    TTF_Font* font = TTF_OpenFont("fonts/videotype.otf", 24);
    TTF_Font* largeFont = TTF_OpenFont("fonts/videotype.otf", 74);

    if (font && largeFont) 
    {
        SDL_Color white = { 255, 255, 255, 255 };
        SDL_Color purple = { 252, 86, 254, 255 };

        char gameOverText[50];
        snprintf(gameOverText, sizeof(gameOverText), "GAME OVER");
        RenderText(gameOverText, SCREEN_WIDTH / 2 - 200, 
            SCREEN_HEIGHT / 2 - 150, largeFont, purple);

        char scoreText[50];
        snprintf(scoreText, sizeof(scoreText), "SCORE: %d", score);
        RenderText(scoreText, SCREEN_WIDTH / 2 - 50, 
            SCREEN_HEIGHT / 2 - 50, font, white);

        char highScoreText[50];
        snprintf(highScoreText, sizeof(highScoreText), "HIGH SCORE: %d", highScore);
        RenderText(highScoreText, SCREEN_WIDTH / 2 - 50, 
            SCREEN_HEIGHT / 2, font, white);

        RenderWindowFrame(purple, SCREEN_WIDTH, SCREEN_HEIGHT);

        TTF_CloseFont(font);
        TTF_CloseFont(largeFont);
    }

    SDL_RenderPresent(renderer);
}

int GameLoop() 
{
    SDL_Event event;
    int running = 1;

    InitGame();

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
                else if (event.key.keysym.sym == SDLK_RETURN && gameOver) 
                {
                    int result = ShowEndGameMenu(score);
                    if (result == 1) 
                    {
                        InitGame();
                        gameOver = false;
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

        if (gameOver) 
        {
            RenderEndScreen();
        }
        else 
        {
            UpdateGame();
            RenderGame();
        }

        SDL_Delay(16);
    }

    return running;
}
