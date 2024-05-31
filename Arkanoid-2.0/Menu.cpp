#include "Menu.h"

// Функция для меню выбора игры
int ShowGameModeMenu(int* gameMode)
{
	SDL_Event event;
	int running = 1;

	TTF_Font* font = TTF_OpenFont("fonts/videotype.otf", 24);
	TTF_Font* titleFont = TTF_OpenFont("fonts/AvalonRegular.ttf", 74);
	TTF_Font* modeFont = TTF_OpenFont("fonts/videotype.otf", 26);

	if (!font || !titleFont)
	{
		SDL_Log("Failed to load font: %s", TTF_GetError());
		return 0;
	}

	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Color blue = { 82, 255, 255, 255 };
	SDL_Color purple = { 252, 86, 254, 255 };
	SDL_Color black = { 0, 0, 0, 255 };

	int currentSelection = 0;
	const char* menuItems[] =
	{
		"Classic",
		"Timed",
		"Back"
	};
	int menuItemCount = sizeof(menuItems) / sizeof(menuItems[0]);

	while (running)
	{
		// Получить размеры окна
		int windowWidth, windowHeight;
		SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

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
		int startX = (windowWidth - maxWidth) / 2;
		int startY = (windowHeight - totalHeight) / 2;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = 0;
				*gameMode = -1; // Выход из игры
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
						*gameMode = 1; // Классический режим
						running = 0;
					}
					else if (currentSelection == 1)
					{
						*gameMode = 2; // Режим на время
						running = 0;
					}
					else if (currentSelection == 2)
					{
						*gameMode = 0; // Назад в меню
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
						*gameMode = 1; // Классический режим
						running = 0;
					}
					else if (currentSelection == 1)
					{
						*gameMode = 2; // Режим на время
						running = 0;
					}
					else if (currentSelection == 2)
					{
						*gameMode = 0; // Назад в меню
						running = 0;
					}
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		RenderBackground(windowWidth, windowHeight);

		// Отрисовка Game Mode
		int titleWidth, titleHeight;
		TTF_SizeText(titleFont, "Game Mode", &titleWidth, &titleHeight);
		int titleX = (windowWidth - titleWidth) / 2;
		int titleY = 110;
		RenderText("Game Mode", titleX, titleY, titleFont, purple);

		// Отображение текущего режима
		const char* currentModeText = (*gameMode == 1) ? "Current Mode: Classic" : "Current Mode: Timed";
		int modeWidth, modeHeight;
		TTF_SizeText(modeFont, currentModeText, &modeWidth, &modeHeight);
		int modeX = (windowWidth - modeWidth) / 2;
		int modeY = titleY + titleHeight;
		RenderText(currentModeText, modeX, modeY, modeFont, blue);

		int y = startY;
		for (int i = 0; i < menuItemCount; i++)
		{
			int width, height;
			TTF_SizeText(font, menuItems[i], &width, &height);
			RenderText(menuItems[i], startX, y, font, (i == currentSelection) ? blue : white);
			y += height + 10;
		}
		RenderButtonFrame(startX - 20, startY - 10, maxWidth + 40, totalHeight + 20, purple);

		RenderWindowFrame(purple, windowWidth, windowHeight);

		SDL_RenderPresent(renderer);
	}

	TTF_CloseFont(font);
	TTF_CloseFont(titleFont);
	TTF_CloseFont(modeFont);
	
	return *gameMode;
}

// Функция для показа правил игры
int ShowGameRulesMenu()
{
	SDL_Event event;
	int running = 1;

	TTF_Font* font = TTF_OpenFont("fonts/videotype.otf", 24);
	TTF_Font* titleFont = TTF_OpenFont("fonts/AvalonRegular.ttf", 74);

	if (!font || !titleFont)
	{
		SDL_Log("Failed to load font: %s", TTF_GetError());
		return 0;
	}

	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Color blue = { 82, 255, 255, 255 };
	SDL_Color purple = { 252, 86, 254, 255 };
	SDL_Color red = { 252, 0, 0, 255 };

	const char* rulesText[] = 
	{
		"Game Rules:",
		"1. Use the arrow keys to move the paddle.",
		"2. Break all the bricks to advance to the next level.",
		"3. Don't let the ball fall below the paddle.",
		"4. Collect power-ups for bonuses.",
		"Press any key for exit"
	};
	int rulesTextCount = sizeof(rulesText) / sizeof(rulesText[0]);

	while (running)
	{
		// Получить размеры окна
		int windowWidth, windowHeight;
		SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = 0;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_ESCAPE)
				{
					running = 0; // Возврат в главное меню
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					running = 0; // Возврат в главное меню при клике
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		RenderBackground(windowWidth, windowHeight);

		// Отрисовка заголовка
		int titleWidth, titleHeight;
		TTF_SizeText(titleFont, "Game Rules", &titleWidth, &titleHeight);
		int titleX = (windowWidth - titleWidth) / 2;
		int titleY = 120;
		RenderText("Game Rules", titleX, titleY, titleFont, purple);

		// Отрисовка текста правил
		int y = titleY + titleHeight + 20;
		for (int i = 0; i < rulesTextCount; i++)
		{
			int width, height;
			TTF_SizeText(font, rulesText[i], &width, &height);
			int x = (windowWidth - width) / 2;

			if (i == 5)
			{
				RenderText(rulesText[i], x, y, font, red);
			}
			else if (i == 0)
			{
				RenderText(rulesText[i], x, y, font, blue);
			}
			else
			{
				RenderText(rulesText[i], x, y, font, white);
			}
			y += height + 10;

			
		}

		RenderWindowFrame(purple, windowWidth, windowHeight);

		SDL_RenderPresent(renderer);
	}

	TTF_CloseFont(font);
	TTF_CloseFont(titleFont);

	return 0; // Возврат в главное меню
}

// Функция для главного меню
int ShowMainMenu(int* inGame, int *gameMode)
{
	SDL_Event event;
	int running = 1;

	TTF_Font* font = TTF_OpenFont("fonts/videotype.otf", 24);
	TTF_Font* font2 = TTF_OpenFont("fonts/ARKANOID.TTF", 90);
	TTF_Font* ownerfont = TTF_OpenFont("fonts/videotype.otf", 15);

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

	while (running)
	{
		// Получаем размеры окна
		int windowWidth, windowHeight;
		SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

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
		int startX = (windowWidth - maxWidth) / 2;
		int startY = (windowHeight - totalHeight) / 2;

		// Центрирование текста
		int arkanoidWidth, arkanoidHeight;
		TTF_SizeText(font2, "Arkanoid", &arkanoidWidth, &arkanoidHeight);
		int arkanoidX = (windowWidth - arkanoidWidth) / 2;
		int arkanoidY = 40;

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
					else if (currentSelection == 1)
					{
						*gameMode = ShowGameModeMenu(gameMode);
						if (*gameMode == -1)
						{
							running = 0;
						}
					}
					else if (currentSelection == 2)
					{
						ShowGameRulesMenu();
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
					else if (currentSelection == 1)
					{
						*gameMode = ShowGameModeMenu(gameMode);
						if (*gameMode == -1)
						{
							running = 0;
						}
					}
					else if (currentSelection == 2)
					{
						ShowGameRulesMenu();
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
			else if (event.type == SDL_WINDOWEVENT)
			{
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);
				}
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		RenderBackground(windowWidth, windowHeight);

		RenderText("Arkanoid", arkanoidX, arkanoidY, font2, purple);
		RenderText("created by cordyy", startX - 4, windowHeight - 25, ownerfont, purple);

		int y = startY;
		for (int i = 0; i < menuItemCount; i++) 
		{
			int width, height;
			TTF_SizeText(font, menuItems[i], &width, &height);
			RenderText(menuItems[i], startX, y, font, (i == currentSelection) ? blue : white);
			y += height + 10;
		}
		RenderButtonFrame(startX - 20, startY - 10, maxWidth + 40, totalHeight + 20, purple);

		RenderWindowFrame(purple, windowWidth, windowHeight);

		SDL_RenderPresent(renderer);
	}

	TTF_CloseFont(font);
	TTF_CloseFont(font2);
	TTF_CloseFont(ownerfont);

	if (*inGame)
	{
		return 1;
	}
	
	return running;
}
