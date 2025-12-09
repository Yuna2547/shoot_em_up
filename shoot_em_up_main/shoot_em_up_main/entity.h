#pragma once

#include <SDL3/SDL.h>

// Forward declaration
class Sprite;

class Entity {
public:
    SDL_FRect rect;
    float speed;
    int screen_width;
    int screen_height;
    Sprite* sprite;
    int health;
    int max_health;
    float invulnerable_timer;
    int offset_x;

    Entity();
    Entity(float x, float y, float w, float h, float speed_, SDL_Renderer* renderer);
    ~Entity();

    void Init(float x, float y, float w, float h, float speed_, SDL_Renderer* renderer);
    void update(const bool* keys, float dt);
    void setScreenBounds(int width, int height);
    void setOffsetX(int offset);
    void draw(SDL_Renderer* renderer) const;
    void takeDamage(int amount);
    void resetPosition(float x, float y);
    void resetHealth();

    const SDL_FRect& getRect() const;
    int getHealth() const;
    bool isInvulnerable() const;
};