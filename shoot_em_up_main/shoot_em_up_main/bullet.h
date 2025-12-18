#pragma once

#include <SDL3/SDL.h>
#include <vector>

class Bullet {
public:
    SDL_FRect rect;
    float speed;
    bool active;

    Bullet();

    void update(float dt);
    void draw(SDL_Renderer* renderer) const;
    void deactivate();
    const SDL_FRect& getRect() const;
};

class BulletManager {
private:
    std::vector<Bullet> bullets;
    int max_bullets;
    float shoot_cooldown;
    float cooldown_timer;

public:
    BulletManager(int maxBullets = 100, float cooldown = 0.1f);

    void update(float dt);
    void shoot(float x, float y);
    void updateBullets(float dt);
    void reset();
    void draw(SDL_Renderer* renderer) const;

    std::vector<Bullet>& getBullets();
};

class EnemyBullet {
public:
    SDL_FRect rect;
    float speed;
    bool active;

    EnemyBullet();

    void update(float dt, int screen_height);
    void draw(SDL_Renderer* renderer) const;
    void deactivate();
    const SDL_FRect& getRect() const;
};

class EnemyBulletManager {

private:
    std::vector<EnemyBullet> bullets;
    int max_bullets;
    float shoot_cooldown;
    float cooldown_timer;

public:
    EnemyBulletManager(int maxBullets, float cooldown = 0.5f);

    void update(float dt);

    bool canShoot() const;

    void shoot(float x, float y);
    void updateBullets(float dt, int screen_height);
    void draw(SDL_Renderer* render);
    void reset();

    std::vector<EnemyBullet>& getBullets();
};
