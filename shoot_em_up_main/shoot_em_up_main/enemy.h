#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include "Sprite.h"

enum class EnemyType {
    tomato,
    brocolie,
    carrot
};

class Enemy {
private:
    SDL_FRect rect;
    float target_y;
    float speed;
    EnemyType type;
    bool in_position;
    bool has_collided;
    Sprite* sprite;

public:
    Enemy(float x, float start_y, float target_y, float w, float h, float speed, EnemyType type, SDL_Renderer* renderer);
    ~Enemy();

    // Disable copy to avoid sprite pointer issues
    Enemy(const Enemy&) = delete;
    Enemy& operator=(const Enemy&) = delete;

    // Enable move
    Enemy(Enemy&& other) noexcept;
    Enemy& operator=(Enemy&& other) noexcept;

    void update(float dt);
    void draw(SDL_Renderer* renderer) const;

    bool isInPosition() const { return in_position; }
    const SDL_FRect& getRect() const { return rect; }
    EnemyType getType() const { return type; }

    bool hasCollided() const { return has_collided; }
    void setCollided() { has_collided = true; }
};

class EnemyManager {
private:
    std::vector<Enemy> enemies;
    float spawn_timer;
    int next_enemy_index;
    bool all_spawned;
    SDL_Renderer* renderer;

public:
    EnemyManager();

    void setupEnemies(SDL_Renderer* renderer);
    void update(float dt);
    void draw(SDL_Renderer* renderer);
    void reset();

    const std::vector<Enemy>& getEnemies() const { return enemies; }
    std::vector<Enemy>& getEnemies() { return enemies; }
};