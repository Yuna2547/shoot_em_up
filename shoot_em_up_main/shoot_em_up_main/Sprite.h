#ifndef SPRITE_H
#define SPRITE_H

#include <SDL3/SDL.h>

typedef struct {
    SDL_Texture* texture;
    int width;
    int height;
} Sprite;

Sprite* Sprite_Load(SDL_Renderer* renderer, const char* path);
void Sprite_Destroy(Sprite* sprite);
void Sprite_Draw(Sprite* sprite, SDL_Renderer* renderer, SDL_FRect* dstRect);

#endif
