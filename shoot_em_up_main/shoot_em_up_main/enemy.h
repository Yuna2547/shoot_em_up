#pragma once

#include <SDL3/SDL.h>
#include <vector>

enum class EnemyType {
    RED,
    YELLOW,
    BLUE
};

class Enemy {
private:
    SDL_FRect rect;
    float target_y;
    float speed;
    EnemyType type;
    bool in_position;
    bool has_collided;  // ADD THIS

public:
    Enemy(float x, float start_y, float target_y, float w, float h, float speed, EnemyType type);

    void update(float dt);
    void draw(SDL_Renderer* renderer) const;

    bool isInPosition() const { return in_position; }
    const SDL_FRect& getRect() const { return rect; }
    EnemyType getType() const { return type; }

    // ADD THESE TWO METHODS
    bool hasCollided() const { return has_collided; }
    void setCollided() { has_collided = true; }
};

class EnemyManager {
private:
    std::vector<Enemy> enemies;
    float spawn_timer;
    int next_enemy_index;
    bool all_spawned;

public:
    EnemyManager();

    void setupEnemies();
    void update(float dt);
    void draw(SDL_Renderer* renderer);
    void reset();

    const std::vector<Enemy>& getEnemies() const { return enemies; }
    std::vector<Enemy>& getEnemies() { return enemies; }  // ADD THIS NON-CONST VERSION
};