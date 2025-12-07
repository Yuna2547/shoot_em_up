#pragma once

#include <SDL3/SDL.h>

class Entity {
private:
    SDL_FRect rect;
    float speed;
    int screen_width;
    int screen_height;

public:
    // Constructor
    Entity(float x, float y, float w, float h, float speed);

    // Update position based on keyboard input
    void update(const bool* keys, float dt);

    // Set screen bounds
    void setScreenBounds(int width, int height);

    // Draw entity
    void draw(SDL_Renderer* renderer) const;

    // Accessors
    const SDL_FRect& getRect() const { return rect; }
};




