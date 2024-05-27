#include "InitDeInit.h"

void DeInit(int error)
{
	if (renderer != NULL)
	{
		SDL_DestroyRenderer(renderer);
	}
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}
	Mix_CloseAudio();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	//exit(error);
}

void Init()
{
	// Инициализация библиотеки SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		printf("Couldn`t init SDL! Error: %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}

	// Инициализация библиотеки SDL_image
	int result;
	if ((result = IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) == 0)
	{
		printf("Couldn`t init SDL_image! Error: %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}
	if (result && IMG_INIT_PNG)
	{
		printf("Initialized PNG library!\n");
	}
	else
	{
		printf("Couldn`t init PNG!\n");
	}
	if (result && IMG_INIT_JPG)
	{
		printf("Initialized JPG library!\n");
	}
	else
	{
		printf("Couldn`t init JPG!\n");
	}

	// Инициализация библиотеки SDL_ttf
	if (TTF_Init())
	{
		printf("Couldn`t init SDL_TTF! Error: %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}

	window = SDL_CreateWindow("ARKANOID", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	// Инициализация библиотеки SDL_mixer
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		printf("Couldn`t init SDL_mixer! Error: %s", SDL_GetError());
		DeInit(1);
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! Error: %s\n", Mix_GetError());
		DeInit(1);
	}

	if (window == NULL)
	{
		printf("Couldn`t create Window! Error: %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("Couldn`t create rendererer! Error: %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}
}