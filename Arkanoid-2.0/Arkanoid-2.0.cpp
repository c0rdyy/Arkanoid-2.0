#include "InitDeInit.h"
#include "Game_Realization.h"

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int main(int argc, char** argv)
{
    Init();

    Realization();

    DeInit(0);
    return 0;
}

