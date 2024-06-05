#include "Menu.h"
#include "settings.h"

// Функция для показа правил игры
int ShowGameRulesMenu()
{
	SDL_Event event;
	int running = 1;

	TTF_Font* font = TTF_OpenFont("fonts/videotype.otf", 24);
	TTF_Font* titleFont = TTF_OpenFont("fonts/AvalonRegular.ttf", 74);

	SDL_Color colors[] = 
	{
	{255, 0, 0, 255},      // Красный
	{0, 255, 0, 255},      // Зеленый
	{0, 0, 255, 255},      // Синий
	{255, 255, 0, 255},    // Желтый
	{82, 255, 255, 255},   // Светло-голубой
	{252, 86, 254, 255},   // Фиолетовый
	{0, 0, 0, 255},        // Черный
	{255, 255, 255, 255},  // Белый
	{252, 0, 0, 255},      // Темно-красный
	{163, 13, 3, 255}      // barn red
	};

	const char* rulesText[] =
	{
		"Game Rules:",
		"1. Use the arrow keys to move the paddle.",
		"2. Break all the bricks to advance to the next level.",
		"3. Don't let the ball fall below the paddle.",
		"4. Collect power-ups for bonuses.",
		"Red bonus - platform increase",
		"Green bonus - extra life",
		"Blue bonus - platform acceleration",
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
					running = 0;
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
		int titleY = 80;
		RenderText("Game Rules", titleX, titleY, titleFont, colors[5]);

		// Отрисовка текста правил
		int y = titleY + titleHeight + 10;

		for (int i = 0; i < rulesTextCount; i++)
		{
			int width, height;
			TTF_SizeText(font, rulesText[i], &width, &height);
			int x = (windowWidth - width) / 2;

			SDL_Color textColor = colors[7];
			if (i == 0)
			{
				textColor = colors[4];
			}
			else if (i == 8)
			{
				textColor = colors[0];
			}

			RenderText(rulesText[i], x, y, font, textColor);
			y += height + 10;

		}

		RenderWindowFrame(colors[5], windowWidth, windowHeight);

		SDL_RenderPresent(renderer);
	}

	TTF_CloseFont(font);
	TTF_CloseFont(titleFont);

	return 0;
}

// Функция для показа рекордов
int ShowHighScores(Game* game) 
{
	SDL_Event event;
	int running = 1;

	TTF_Font* font = TTF_OpenFont("fonts/videotype.otf", 24);
	TTF_Font* fontLarge = TTF_OpenFont("fonts/videotype.otf", 74);

	SDL_Color yellow = { 255, 255, 0, 255 };
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Color blue = { 82, 255, 255, 255 };
	SDL_Color purple = { 252, 86, 254, 255 };

	while (running) 
	{
		while (SDL_PollEvent(&event)) 
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) 
			{
				running = 0;
			}
		}

		RenderBackground(SCREEN_WIDTH, SCREEN_HEIGHT);

		RenderCenteredText("High Scores", fontLarge, purple, SCREEN_HEIGHT / 4 - 50);

		for (int i = 0; i < 5; i++) 
		{
			char scoreText[50];
			sprintf_s(scoreText, "%d. %d", i + 1, game->scores[i]);
			RenderCenteredText(scoreText, font, white, SCREEN_HEIGHT / 2 - 90 + i * 30);
		}
		RenderCenteredText("Press ESC to exit", font, yellow, SCREEN_HEIGHT / 2 + 90);

		SDL_RenderPresent(renderer);
	}

	TTF_CloseFont(font);
	TTF_CloseFont(fontLarge);

	return 0;
}

// Функция для главного меню
int ShowMainMenu(int* inGame, int *gameMode, Game *game)
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
		"Game Rules",
		"High Scores",
		"Sound: ON",
		"Quit"
	};
	int menuItemCount = sizeof(menuItems) / sizeof(menuItems[0]);

	// Обновление текста в зависимости от состояния звука
	if (!globalSettings.soundEnabled) 
	{
		menuItems[3] = "Sound: OFF";
	}

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
		totalHeight -= 10; // Последний отступ

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
					switch (currentSelection) 
					{
					// Начать игру
					case 0: 
						*inGame = 1;
						running = 0;
						break;
					// Правила игры
					case 1:
						ShowGameRulesMenu();
						break;
					// Рекорды
					case 2:
						ShowHighScores(game);
						break;
					// Музыка
					case 3:
						globalSettings.soundEnabled = !globalSettings.soundEnabled;
						SaveSettings(&globalSettings);
						menuItems[3] = globalSettings.soundEnabled ? "Sound: ON" : "Sound: OFF";
						if (globalSettings.soundEnabled) 
						{
							Mix_ResumeMusic();
						}
						else 
						{
							Mix_PauseMusic();
						}
						break;
					// Выход
					case 4:
						running = 0;
						break;
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
					switch (currentSelection)
					{
						// Начать игру
					case 0:
						*inGame = 1;
						running = 0;
						break;
						// Правила игры
					case 1:
						ShowGameRulesMenu();
						break;
						// Рекорды
					case 2:
						ShowHighScores(game);
						break;
						// Музыка
					case 3:
						globalSettings.soundEnabled = !globalSettings.soundEnabled;
						SaveSettings(&globalSettings);
						menuItems[3] = globalSettings.soundEnabled ? "Sound: ON" : "Sound: OFF";
						if (globalSettings.soundEnabled)
						{
							Mix_ResumeMusic();
						}
						else
						{
							Mix_PauseMusic();
						}
						break;
						// Выход
					case 4:
						running = 0;
						break;
					}
					break;
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
