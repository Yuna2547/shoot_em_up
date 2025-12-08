#pragma once

#include <SDL3/SDL.h>

class Sprite {
public:
    // Loads the image at construction. If loading fails, IsValid() will be false.
    Sprite(SDL_Renderer* renderer, const char* path);
    ~Sprite();

    bool IsValid() const;
    int GetWidth() const;
    int GetHeight() const;
    SDL_Texture* GetTexture() const;

    // Draw the sprite to renderer at the destination rectangle (float rect in SDL3)
    void Draw(SDL_Renderer* renderer, SDL_FRect* dstRect);

private:
    SDL_Texture* texture;
    int width;
    int height;
};