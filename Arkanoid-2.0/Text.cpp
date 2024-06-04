#include "Text.h"

void RenderText(const char* text, int x, int y, TTF_Font* font, SDL_Color color)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstrect = { x, y, surface->w, surface->h };

    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    SDL_DestroyTexture(texture);
}

void RenderCenteredText(const char* text, TTF_Font* font, SDL_Color color, int y) 
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int textWidth = surface->w;
    int textHeight = surface->h;

    SDL_Rect render = { (SCREEN_WIDTH - textWidth) / 2, y, textWidth, textHeight };
    SDL_RenderCopy(renderer, texture, NULL, &render);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
