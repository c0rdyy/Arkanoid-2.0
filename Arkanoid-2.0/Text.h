#pragma once

#ifndef TEXT_H
#define TEXT_H

#include "InitDeInit.h"

void RenderText(const char* text, int x, int y, TTF_Font* font, SDL_Color color);
void RenderCenteredText(const char* text, TTF_Font* font, SDL_Color color, int y);

#endif