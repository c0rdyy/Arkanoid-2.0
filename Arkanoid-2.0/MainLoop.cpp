#include "MainLoop.h"

void MainLoop()
{  
    int running = 1;
    int inGame = 0;

    while (running)
    {
        if (!inGame)
        {
            running = ShowMainMenu(&inGame);
        }
        else
        {
            running = GameLoop();
            inGame = 0;
        }
    }
}