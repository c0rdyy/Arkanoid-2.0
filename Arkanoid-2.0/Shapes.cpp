#include "Shapes.h"

void RenderBackground(int windowWidth, int windowHeight)
{
	SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
	SDL_Rect background = { 0, 0, windowWidth, windowHeight };
	SDL_RenderFillRect(renderer, &background);

	SDL_SetRenderDrawColor(renderer, 50, 0, 50, 255);
	for (int i = 0; i < windowHeight; i += 20)
	{
		SDL_RenderDrawLine(renderer, 0, i, windowWidth, i);
	}

	for (int i = 0; i < windowWidth; i += 20)
	{
		SDL_RenderDrawLine(renderer, i, 0, i, windowHeight);
	}
}

void RenderButtonFrame(int x, int y, int w, int h, SDL_Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_Rect rect = { x, y, w, h };
	SDL_RenderDrawRect(renderer, &rect);
}

void RenderWindowFrame(SDL_Color color, int windowWidth, int windowHeight)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	for (int i = 0; i < 3; i++)
	{
		// Верхняя линия
		SDL_RenderDrawLine(renderer, 0, 0 + i, windowWidth, 0 + i);
		// Левая линия
		SDL_RenderDrawLine(renderer, 0 + i, 0, 0 + i, windowHeight);
		// Правая линия
		SDL_RenderDrawLine(renderer, windowWidth - i, 0, windowWidth - i, windowHeight);
		// Нижняя линия
		SDL_RenderDrawLine(renderer, 0, windowHeight - i, windowWidth, windowHeight - i);
	}

}