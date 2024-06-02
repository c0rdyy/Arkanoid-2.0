#pragma once

#ifndef GAME_H
#define GAME_H

#include "InitDeInit.h"
#include "Text.h"
#include "stdlib.h"
#include "time.h"
#include "Shapes.h"

const int MAX_BRICKS = 50;
const int MAX_POWERUPS = 10;

struct Paddle 
{
    int x, y, w, h;
    int speed;
};

struct Ball 
{
    int x, y, w, h;
    int speedX, speedY;
};

struct Brick 
{
    int x, y, w, h;
    SDL_Color color;
    bool destroyed;
};

struct PowerUp 
{
    int x, y, w, h;
    int type;
    bool active;
};

struct Game 
{
    Paddle paddle;
    Ball ball;
    Brick bricks[MAX_BRICKS];
    PowerUp powerUps[MAX_POWERUPS];
    int powerUpCount;
    int score, highScore, lives, currentRound;
    bool gameOver, ballLaunched;
};

int GameLoop(Game* game);
int ShowEndGameMenu(int score, int highScore);

void InitGame(Game* game);
void AddPowerUp(Game* game, int x, int y, int type);
void UpdateGame(Game* game);
void RenderGame(Game* game);

#endif
