#pragma once

#include <SDL3/SDL.h>
#include "Sprite.h"

class Entity {
public:
    // Keep these public so existing code that accesses player.rect etc. continues to work
    SDL_FRect rect;
    float speed;
    int screen_width;
    int screen_height;
    Sprite* sprite;

    // Constructors / destructor
    Entity();
    Entity(float x, float y, float w, float h, float speed, SDL_Renderer* renderer);
    ~Entity();

    // Methods
    void Init(float x, float y, float w, float h, float speed, SDL_Renderer* renderer);
    void Update(const bool* keys, float dt);
    void SetScreenBounds(int width, int height);
    void Draw(SDL_Renderer* renderer);
};



