#pragma once

#ifndef INITDEINIT_H
#define INITDEINIT_H

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "settings.h"

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern Settings globalSettings;

void DeInit(int error);
void Init();

#endif