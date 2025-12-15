#include "Sprite.h"
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL.h>

Sprite::Sprite(SDL_Renderer* renderer, const char* path)
    : texture(nullptr), width(0), height(0)
{
    if (!renderer || !path) 
        return;

    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        SDL_Log("Error loading image from ",path, " : ",  SDL_GetError());
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Error creating texture from ",path, " : ",  SDL_GetError());
        SDL_DestroySurface(surface);
        return;
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    width = surface->w;
    height = surface->h;

    SDL_DestroySurface(surface);
}

Sprite::~Sprite() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

bool Sprite::IsValid() const {
    return texture != nullptr;
}

int Sprite::GetWidth() const {
    return width;
}

int Sprite::GetHeight() const {
    return height;
}

SDL_Texture* Sprite::GetTexture() const {
    return texture;
}

void Sprite::Draw(SDL_Renderer* renderer, const SDL_FRect* dstRect) const {
    if (!renderer || !texture || !dstRect) 
        return;
    SDL_RenderTexture(renderer, texture, nullptr, dstRect);
}