#pragma once

#ifndef TEXT_H
#define TEXT_H

#include "InitDeInit.h"

void RenderText(const char* text, int x, int y, TTF_Font* font, SDL_Color color);
SDL_Texture* LoadTexture(const char* file);
void RenderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h);


#endif