#pragma once
#include <SDL3/SDL.h>

class Sprite {
private:
    SDL_Texture* texture;
    int width;
    int height;

public:
    Sprite(SDL_Renderer* renderer, const char* path);
    ~Sprite();

    // Delete copy constructor and assignment
    Sprite(const Sprite&) = delete;
    Sprite& operator=(const Sprite&) = delete;

    bool IsValid() const;
    int GetWidth() const;
    int GetHeight() const;
    SDL_Texture* GetTexture() const;
    void Draw(SDL_Renderer* renderer, const SDL_FRect* dstRect) const;
};
