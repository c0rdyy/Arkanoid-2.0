#pragma once

#ifndef MENU_H
#define MENU_H

#include "InitDeInit.h"
#include "Text.h"
#include "Shapes.h"

int ShowMainMenu(int* inGame, int* gameMode);
int ShowGameModeMenu(int* gameMode);
int ShowGameRulesMenu();

#endif