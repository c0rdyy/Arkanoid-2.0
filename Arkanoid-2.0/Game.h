#pragma once

#ifndef GAME_H
#define GAME_H

#include "InitDeInit.h"

#define BRICK_ROWS 5
#define BRICK_COLS 10

typedef struct {
    float x, y;
    float dx, dy;
    int width, height;
} Ball;

typedef struct {
    float x, y;
    int width, height;
    float speed;
} Paddle;

typedef struct {
    float x, y;
    int width, height;
    int destroyed;
} Brick;

void InitBall(Ball* ball);
void InitPaddle(Paddle* paddle);
void InitBricks(Brick bricks[], int rows, int cols);

void UpdateBall(Ball* ball, Paddle* paddle, Brick bricks[], int brickCount);
void UpdatePaddle(Paddle* paddle);

void RenderBall(SDL_Renderer* renderer, Ball* ball);
void RenderPaddle(SDL_Renderer* renderer, Paddle* paddle);
void RenderBricks(SDL_Renderer* renderer, Brick bricks[], int brickCount);

int GameLoop(); // Объявление функции игрового цикла

#endif