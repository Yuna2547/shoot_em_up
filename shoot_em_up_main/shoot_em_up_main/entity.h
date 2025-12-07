#pragma once

#include <SDL3/SDL.h>

class Entity {
private:
    SDL_FRect rect;
    float speed;
    int screen_width;
    int screen_height;
    int health;
    int max_health;
    float invulnerable_timer;

public:
    // Constructor
    Entity(float x, float y, float w, float h, float speed);

    // Update position based on keyboard input
    void update(const bool* keys, float dt);

    // Set screen bounds
    void setScreenBounds(int width, int height);

    // Draw entity and health bar
    void draw(SDL_Renderer* renderer) const;

    // Health management
    void takeDamage(int amount);
    int getHealth() const { return health; }
    int getMaxHealth() const { return max_health; }
    bool isInvulnerable() const { return invulnerable_timer > 0.0f; }

    // Accessors
    const SDL_FRect& getRect() const { return rect; }
};