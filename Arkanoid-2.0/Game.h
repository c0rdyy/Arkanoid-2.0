#pragma once

#ifndef GAME_H
#define GAME_H

#include "InitDeInit.h"
#include "Text.h"
#include "stdlib.h"
#include "time.h"
#include "Shapes.h"
#include <math.h>

const int MAX_BRICKS = 150;
const int MAX_POWERUPS = 10;
const int MAX_LEVELS = 3;

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
    int hitPoints;
};

struct PowerUp 
{
    int x, y, w, h;
    int type;
    bool active;
    Uint32 activationTime;
    Uint32 duration;
};

struct Game 
{
    Paddle paddle;
    Ball ball;
    Brick bricks[MAX_BRICKS];
    PowerUp powerUps[MAX_POWERUPS];
    Mix_Music* gameMusic = NULL;
    int powerUpCount;
    int score, highScore, lives;
    bool gameOver, victory;
    bool ballLaunched;
    bool musicOn;
    int currentLevel;
    int brickCount;
    int paddleWidthTimer;
    int paddleSpeedTimer;
    int ballSpeed;
};

void InitGame(Game* game);
void InitLevel(Game* game);

void InitLevel1(Game* game);
void InitLevel2(Game* game);
void InitLevel3(Game* game);

int ShowEndGameMenu(Game* game);
int ShowVictoryMenu(Game* game);
int ShowPauseMenu(Game* game);

void UpdateGame(Game* game);
void RenderGame(Game* game);

int GameLoop(Game* game);

void AddPowerUp(Game* game, int x, int y, int type);
bool AllBricksDestroyed(Game* game);
void ResetBallAndPaddle(Game* game);
int CountRemainingBricks(Game* game);

#endif // !GAME_H
