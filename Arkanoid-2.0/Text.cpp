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

SDL_Texture* LoadTexture(const char* file) 
{
    SDL_Texture* texture = IMG_LoadTexture(renderer, file);
    if (texture == NULL) 
    {
        SDL_Log("Unable to load texture: %s", IMG_GetError());
    }
    return texture;
}

void RenderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h) 
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(ren, tex, NULL, &dst);
}