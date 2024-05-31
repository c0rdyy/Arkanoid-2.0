#include "Game.h"

Ball ball;
Paddle paddle;
Brick bricks[BRICK_ROWS * BRICK_COLS];

void InitGame()
{
    InitBall(&ball);
    InitPaddle(&paddle);
    InitBricks(bricks, BRICK_ROWS, BRICK_COLS);
}

int GameLoop()
{
    InitGame();

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
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = 0;
                }
            }
        }

        UpdatePaddle(&paddle);
        UpdateBall(&ball, &paddle, bricks, BRICK_ROWS * BRICK_COLS);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        RenderPaddle(renderer, &paddle);
        RenderBall(renderer, &ball);
        RenderBricks(renderer, bricks, BRICK_ROWS * BRICK_COLS);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }
    return running;
}
