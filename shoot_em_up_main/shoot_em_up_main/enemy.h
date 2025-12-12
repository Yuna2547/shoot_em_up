#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include "Sprite.h"

enum class EnemyType {
    tomato,
    broccoli,
    carrot
};

class Enemy {
public:
    SDL_FRect rect;
    float speed;
    EnemyType type;
    bool has_collided;
    int health;
    int max_health;
    Sprite* sprite;

    // Horizontal movement
    bool horizontal;
    float hspeed;
    int min_x;
    int max_x;
    bool move_right;

    Enemy(float x, float start_y, float w, float h, float speed,
        EnemyType type, SDL_Renderer* renderer);
    ~Enemy();

    // Move constructor and assignment
    Enemy(Enemy&& other) noexcept;
    Enemy& operator=(Enemy&& other) noexcept;

    // Delete copy constructor and assignment
    Enemy(const Enemy&) = delete;
    Enemy& operator=(const Enemy&) = delete;

    void setHorizontalBounds(int minX, int maxX);
    void setHorizontalMovement(bool enabled, float speed, int minX, int maxX);
    void update(float dt);
    void draw(SDL_Renderer* renderer) const;
    void takeDamage(int amount);

    bool isAlive() const;
    bool hasCollided() const;
    void setCollided();
    const SDL_FRect& getRect() const;
    bool isOffScreen(int screen_height) const;
};

class EnemyManager {
private:
    std::vector<Enemy> enemies;
    float spawn_timer;
    size_t next_enemy_index;
    bool all_spawned;
    SDL_Renderer* renderer;
    int play_area_x;
    int play_area_width;

public:
    EnemyManager();

    void setupEnemies(SDL_Renderer* renderer, int play_x, int play_width);
    void update(float dt);
    void draw();
    void reset();
    bool allDestroyed() const;

    std::vector<Enemy>& getEnemies();
};