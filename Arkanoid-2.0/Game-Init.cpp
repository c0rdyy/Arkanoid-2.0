#include "Game.h"

// ������� ������������� ������ 1
void InitLevel1(Game* game)
{
    /*int brickRows = 7;
    int brickCols = 7;*/

    int brickRows = 7;
    int brickCols = 7;

    int brickWidth = 70;
    int brickHeight = 25;
    int padding = 6;

    // ������ ����� ������ ��������
    int totalBricksWidth = brickCols * (brickWidth + padding) - padding;

    // ������ ��������� ������� �� x � y
    int startX = (SCREEN_WIDTH - totalBricksWidth) / 2;
    int startY = 60;

    SDL_Color colors[3] =
    {
        {255, 0, 0, 255},    // �������
        {255, 165, 0, 255},  // ���������
        {255, 255, 0, 255},  // Ƹ����
    };

    game->brickCount = 0;

    for (int i = 0; i < brickRows; i++)
    {
        SDL_Color currentColor = colors[rand() % 3];
        for (int j = 0; j < brickCols; j++)
        {
            if (game->brickCount >= MAX_BRICKS)
            {
                break;
            }

            game->bricks[game->brickCount] =
            {
                startX + j * (brickWidth + padding),
                startY + i * (brickHeight + padding),
                brickWidth,
                brickHeight,
                currentColor,
                false,
                1
            };
            game->brickCount++;
        }
    }
}

// ������� ������������� ������ 2 (��������� �����)
void InitLevel2(Game* game)
{
    /*int brickCols = 8;
    int brickRows = 8;*/

    int brickCols = 8;
    int brickRows = 8;

    int brickWidth = 70;
    int brickHeight = 25;

    int padding = 6;

    SDL_Color colors[3] =
    {
        {251, 0, 255, 255},  // ����������
        {0, 208, 255, 255},    // �������
        {255, 0, 136, 255}    // ���������
    };

    int totalBricksWidth = brickCols * (brickWidth + padding) - padding;
    int startX = (SCREEN_WIDTH - totalBricksWidth) / 2;
    int startY = 60;

    for (int i = 0; i < brickRows; i++)
    {
        SDL_Color currentColor = colors[rand() % 3];

        int rowStartX = startX + (i % 2 == 0 ? 0 : brickWidth / 2);

        for (int j = 0; j < brickCols; j++)
        {
            if (game->brickCount >= MAX_BRICKS)
            {
                break;
            }
            int hitPoints = 1 + (i % 2);
            game->bricks[game->brickCount] =
            {
                rowStartX + j * (brickWidth + padding),
                startY + i * (brickHeight + padding),
                brickWidth,
                brickHeight,
                currentColor,
                false,
                hitPoints
            };
            game->brickCount++;
        }
    }
}

// ������� ������������� ������ 3
void InitLevel3(Game* game)
{
    int brickCols = 8;
    int brickRows = 8;

    int brickWidth = 70;
    int brickHeight = 25;

    int padding = 6;

    SDL_Color destructibleColor = { 192, 192, 192, 255 };  // ������� ���� ��� ����������� ������
    SDL_Color indestructibleColor = { 64, 64, 64, 255 };  // ������ ���� ��� ������������� ������

    int totalBricksWidth = brickCols * (brickWidth + padding) - padding;
    int startX = (SCREEN_WIDTH - totalBricksWidth) / 2;
    int startY = 60;

    // ������ ��� �����������, ��� ������ ���� ������
    int maze[8][8] =
    {
        {2, 1, 2, 2, 1, 2, 2, 2},
        {1, 0, 0, 1, 0, 0, 0, 1},
        {2, 0, 1, 1, 0, 1, 1, 1},
        {1, 0, 1, 2, 2, 0, 0, 1},
        {2, 2, 1, 1, 1, 1, 0, 2},
        {1, 0, 0, 0, 0, 2, 0, 1},
        {2, 1, 1, 1, 0, 1, 2, 2},
        {1, 1, 1, 2, 2, 1, 1, 1}
    };

    for (int i = 0; i < brickRows; i++)
    {
        for (int j = 0; j < brickCols; j++)
        {
            if (maze[i][j] != 0)
            {
                if (game->brickCount >= MAX_BRICKS)
                {
                    break;
                }
                int hitPoints = (maze[i][j] == 1) ? 1 : 3;
                SDL_Color currentColor = (maze[i][j] == 1) ? destructibleColor : indestructibleColor;
                game->bricks[game->brickCount] =
                {
                    startX + j * (brickWidth + padding),
                    startY + i * (brickHeight + padding),
                    brickWidth,
                    brickHeight,
                    currentColor,
                    false,
                    hitPoints
                };
                game->brickCount++;
            }
        }
    }
}

// ����� ������� ������������� �������
void InitLevel(Game* game)
{
    // ���������� �������� ����
    game->ballSpeed = 6 + game->currentLevel - 1;

    if (game->paddleWidthTimer > 0)
    {
        game->paddle.w -= 40;
        game->paddleWidthTimer = 0;
    }
    if (game->paddleSpeedTimer > 0)
    {
        game->paddle.speed -= 3;
        game->paddleSpeedTimer = 0;
    }

    switch (game->currentLevel)
    {
    case 1:
        InitLevel1(game);
        break;
    case 2:
        InitLevel2(game);
        break;
    case 3:
        InitLevel3(game);
        break;
    default:
        InitLevel1(game);
        break;
    }
}

// ������� ������������� ����
void InitGame(Game* game)
{
    srand(time(NULL));

    // ��������� ��������� ��������
    game->gameOver = false;
    game->victory = false;
    game->musicOn = true;
    game->score = 0;
    game->highScore = 0;
    game->lives = 3;
    game->ballLaunched = false;
    game->powerUpCount = 0;
    game->ballSpeed = 6;
    game->currentLevel = 1;

    // ������� ��������
    for (int i = 0; i < MAX_BRICKS; i++)
    {
        game->bricks[i].destroyed = true;
        game->bricks[i].hitPoints = 1;
    }

    InitLevel(game);

    // ������������� ���������� ���������
    game->paddle = { 350, 550, 100, 20, 10 };

    // ������������� ���������� ����
    game->ball = { 350, 530, 10, 10, 0, 0 };

    // ������� �������
    for (int i = 0; i < MAX_POWERUPS; i++)
    {
        game->powerUps[i].active = false;
    }

    // ������������� �������
    for (int i = 0; i < MAX_POWERUPS; i++)
    {
        game->powerUps[i].active = false;
    }
}