#include "Sprite.h"
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL.h>
#include <iostream>

Sprite::Sprite(SDL_Renderer* renderer, const char* path)        //parameters, check if texture loaded
    : texture(nullptr), width(0), height(0){
    if (!renderer || !path) 
        return;

    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        printf("Error loading image");
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Error creating texture");
        SDL_DestroySurface(surface);
        return;
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    width = surface->w;
    height = surface->h;

    SDL_DestroySurface(surface);
}

Sprite::~Sprite() {     //destructor
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