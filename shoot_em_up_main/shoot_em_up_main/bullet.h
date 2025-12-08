#pragma once

#include <SDL3/SDL.h>
#include <vector>

class Bullet {
public:
    SDL_FRect rect;
    float speed;
    bool active;

    Bullet();
    void update(float dt, int screen_height);
    void draw(SDL_Renderer* renderer) const;

    const SDL_FRect& getRect() const { return rect; }
    void deactivate() { active = false; }
};

class BulletManager {
private:
    std::vector<Bullet> bullets;
    int max_bullets;
    float shoot_cooldown;
    float cooldown_timer;

public:
    BulletManager(int maxBullets = 100, float cooldown = 0.3f);

    void update(float dt);
    void shoot(float x, float y);
    void updateBullets(float dt, int screen_height);
    void draw(SDL_Renderer* renderer);

    std::vector<Bullet>& getBullets() { return bullets; }
};