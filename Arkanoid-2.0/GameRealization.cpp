#include "Game.h"

void InitBall(Ball* ball)
{
    ball->x = SCREEN_WIDTH / 2.0f;
    ball->y = SCREEN_HEIGHT / 2.0f;
    ball->dx = 4.0f;
    ball->dy = 4.0f;
    ball->width = 10;
    ball->height = 10;
}

void InitPaddle(Paddle* paddle)
{
    paddle->x = SCREEN_WIDTH / 2.0f - 50;
    paddle->y = SCREEN_HEIGHT - 30;
    paddle->width = 100;
    paddle->height = 20;
    paddle->speed = 10.0f;
}

void InitBricks(Brick bricks[], int rows, int cols)
{
    int brickWidth = SCREEN_WIDTH / cols;
    int brickHeight = 20;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int index = i * cols + j;
            bricks[index].x = j * brickWidth;
            bricks[index].y = i * brickHeight + 50; // отступ сверху
            bricks[index].width = brickWidth - 2; // небольшой отступ между кирпичами
            bricks[index].height = brickHeight - 2;
            bricks[index].destroyed = 0;
        }
    }
}

void UpdateBall(Ball* ball, Paddle* paddle, Brick bricks[], int brickCount)
{
    ball->x += ball->dx;
    ball->y += ball->dy;

    // Столкновение с краями окна
    if (ball->x <= 0 || ball->x + ball->width >= SCREEN_WIDTH)
    {
        ball->dx = -ball->dx;
    }
    if (ball->y <= 0)
    {
        ball->dy = -ball->dy;
    }
    if (ball->y + ball->height >= SCREEN_HEIGHT)
    {
        // Шарик ушел за нижний край, сбросить игру
        InitBall(ball);
        InitPaddle(paddle);
    }

    // Столкновение с платформой
    if (ball->x < paddle->x + paddle->width && ball->x + ball->width > paddle->x &&
        ball->y < paddle->y + paddle->height && ball->y + ball->height > paddle->y)
    {
        ball->dy = -ball->dy;
    }

    // Столкновение с кирпичами
    for (int i = 0; i < brickCount; i++)
    {
        if (!bricks[i].destroyed && ball->x < bricks[i].x + bricks[i].width && ball->x + ball->width > bricks[i].x &&
            ball->y < bricks[i].y + bricks[i].height && ball->y + ball->height > bricks[i].y)
        {
            ball->dy = -ball->dy;
            bricks[i].destroyed = 1;
            break;
        }
    }
}

void UpdatePaddle(Paddle* paddle)
{
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_LEFT])
    {
        paddle->x -= paddle->speed;
    }
    if (state[SDL_SCANCODE_RIGHT])
    {
        paddle->x += paddle->speed;
    }

    // Ограничение движения платформы краями окна
    if (paddle->x < 0)
    {
        paddle->x = 0;
    }
    if (paddle->x + paddle->width > SCREEN_WIDTH)
    {
        paddle->x = SCREEN_WIDTH - paddle->width;
    }
}

void RenderBall(SDL_Renderer* renderer, Ball* ball)
{
    SDL_Rect rect = { (int)ball->x, (int)ball->y, ball->width, ball->height };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void RenderPaddle(SDL_Renderer* renderer, Paddle* paddle)
{
    SDL_Rect rect = { (int)paddle->x, (int)paddle->y, paddle->width, paddle->height };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void RenderBricks(SDL_Renderer* renderer, Brick bricks[], int brickCount)
{
    for (int i = 0; i < brickCount; i++)
    {
        if (!bricks[i].destroyed)
        {
            SDL_Rect rect = { (int)bricks[i].x, (int)bricks[i].y, bricks[i].width, bricks[i].height };
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // красные кирпичи
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}