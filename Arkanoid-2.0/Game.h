#pragma once

#ifndef GAME_H
#define GAME_H

#include "InitDeInit.h"
#include "Text.h"
#include "stdlib.h"
#include "time.h"
#include "Shapes.h"

const int MAX_BRICKS = 50;

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

int GameLoop();
int ShowEndGameMenu(int score);
void RenderEndScreen();

#endif
