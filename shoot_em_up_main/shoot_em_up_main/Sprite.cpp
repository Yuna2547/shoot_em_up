#include "sprite.h"
#include <stdlib.h>
#include <SDL3_image/SDL_image.h>

Sprite* Sprite_Load(SDL_Renderer* renderer, const char* path) {
    // Load PNG (or other formats supported by SDL_image)
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        SDL_Log("Error loading image %s: %s", path, SDL_GetError());
        return NULL;
    }

    Sprite* sprite = (Sprite*)malloc(sizeof(Sprite));
    if (!sprite) {
        SDL_Log("Error allocating memory for sprite");
        SDL_DestroySurface(surface);
        return NULL;
    }

    sprite->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!sprite->texture) {
        SDL_Log("Error creating texture from %s: %s", path, SDL_GetError());
        SDL_DestroySurface(surface);
        free(sprite);
        return NULL;
    }

    // Keep pixel art crisp when scaled
    SDL_SetTextureScaleMode(sprite->texture, SDL_SCALEMODE_NEAREST);

    sprite->width = surface->w;
    sprite->height = surface->h;

    SDL_DestroySurface(surface);
    return sprite;
}

void Sprite_Destroy(Sprite* sprite) {
    if (sprite) {
        if (sprite->texture) {
            SDL_DestroyTexture(sprite->texture);
        }
        free(sprite);
    }
}

void Sprite_Draw(Sprite* sprite, SDL_Renderer* renderer, SDL_FRect* dstRect) {
    if (sprite && sprite->texture) {
        SDL_RenderTexture(renderer, sprite->texture, NULL, dstRect);
    }
}
