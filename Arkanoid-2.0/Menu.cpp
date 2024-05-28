#include "Menu.h"

void RenderButtonFrame(int x, int y, int w, int h, SDL_Color color) 
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_Rect rect = { x, y, w, h };
	SDL_RenderDrawRect(renderer, &rect);
}

void RenderWindowFrame(SDL_Color color) 
{
	for (int i = 0; i < 3; i++)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		// Верхняя линия
		SDL_RenderDrawLine(renderer, 0, 0 + i, 800, 0 + i);
		// Левая линия
		SDL_RenderDrawLine(renderer, 0 + i, 0, 0 + i, 600);
		// Правая линия
		SDL_RenderDrawLine(renderer, 800 - i, 0, 800 - i, 600);
		// Нижняя линия
		SDL_RenderDrawLine(renderer, 0, 600 - i, 800, 600 - i);
	}
	
}

int ShowMainMenu(int* inGame)
{
	SDL_Event event;
	int running = 1;

	TTF_Font* font = TTF_OpenFont("fonts/videotype.otf", 24);
	TTF_Font* font2 = TTF_OpenFont("fonts/ARKANOID.TTF", 90);
	TTF_Font* ownerfont = TTF_OpenFont("fonts/videotype.otf", 15);

	if (!font || !font2) 
	{
		SDL_Log("Failed to load font: %s", TTF_GetError());
		return 0;
	}

	SDL_Color yellow = { 255, 255, 0, 255 };
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Color barn_red = { 163, 13, 3, 255 };
	SDL_Color red = { 255, 13, 3, 255 };
	SDL_Color blue = { 82, 255, 255, 255 };
	SDL_Color purple = { 252, 86, 254, 255 };

	int currentSelection = 0;
	const char* menuItems[] = 
	{
		"Start Game",
		"Game Mode",
		"Game Rules",
		"High Scores",
		"Sound: ON",
		"Quit"
	};
	int menuItemCount = sizeof(menuItems) / sizeof(menuItems[0]);

	int currentScore = 0; // Текущий счёт
	int highScore = 1000; // Рекордный счёт

	// Определение размеров текста
	int totalHeight = 0;
	int textWidth = 0;
	int maxWidth = 0;
	for (int i = 0; i < menuItemCount; i++) 
	{
		int width, height;
		TTF_SizeText(font, menuItems[i], &width, &height);
		totalHeight += height + 10;
		if (width > maxWidth) 
		{
			maxWidth = width;
		}
	}
	totalHeight -= 10; // Убираем последний отступ

	// Координаты для центрирования
	int startX = (800 - maxWidth) / 2;
	int startY = (600 - totalHeight) / 2;

	// Центрирование текста "Arkanoid"
	int arkanoidWidth, arkanoidHeight;
	TTF_SizeText(font2, "Arkanoid", &arkanoidWidth, &arkanoidHeight);
	int arkanoidX = (800 - arkanoidWidth) / 2;
	int arkanoidY = 40;

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = 0;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					currentSelection = (currentSelection - 1 + menuItemCount) % menuItemCount;
					break;
				case SDLK_DOWN:
					currentSelection = (currentSelection + 1) % menuItemCount;
					break;
				case SDLK_RETURN:
					if (currentSelection == 0)
					{
						*inGame = 1;
						running = 0;
					}
					else if (currentSelection == 4)
					{
						// Переключение звука
						menuItems[4] = (menuItems[4] == "Sound: ON") ? "Sound: OFF" : "Sound: ON";
					}
					else if (currentSelection == 5)
					{
						running = 0;
					}
					break;
				}
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				int mouseX = event.motion.x;
				int mouseY = event.motion.y;
				int y = startY;
				for (int i = 0; i < menuItemCount; i++)
				{
					int width, height;
					TTF_SizeText(font, menuItems[i], &width, &height);
					if (mouseX >= startX && mouseX <= startX + width && mouseY >= y && mouseY <= y + height)
					{
						currentSelection = i;
					}
					y += height + 10;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					if (currentSelection == 0)
					{
						*inGame = 1;
						running = 0;
					}
					else if (currentSelection == 4)
					{
						// Переключение звука
						menuItems[4] = (menuItems[4] == "Sound: ON") ? "Sound: OFF" : "Sound: ON";
					}
					else if (currentSelection == 5)
					{
						running = 0;
					}
				}
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		RenderText("Arkanoid", arkanoidX, arkanoidY, font2, purple);
		RenderText("created by cordyy", startX - 4, SCREEN_HEIGHT - 25, ownerfont, purple);
		/*RenderText("Current Score: ", 300, 150, font, white);
		char scoreStr[12];
		sprintf_s(scoreStr, "%d", currentScore);
		RenderText(scoreStr, 500, 150, font, white);
		RenderText("High Score: ", 300, 200, font, white);
		char highScoreStr[12];
		sprintf_s(highScoreStr, "%d", highScore);
		RenderText(highScoreStr, 500, 200, font, white);*/

		int y = startY;
		for (int i = 0; i < menuItemCount; i++) 
		{
			int width, height;
			TTF_SizeText(font, menuItems[i], &width, &height);
			RenderText(menuItems[i], startX, y, font, (i == currentSelection) ? blue : white);
			y += height + 10;
		}
		RenderButtonFrame(startX - 20, startY - 10, maxWidth + 40, totalHeight + 20, purple);

		RenderWindowFrame(purple);

		SDL_RenderPresent(renderer);
	}

	TTF_CloseFont(font);
	TTF_CloseFont(font2);
	TTF_CloseFont(ownerfont);
	return running;
}
