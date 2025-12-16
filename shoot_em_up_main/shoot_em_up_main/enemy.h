#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include "Sprite.h"
#include "bullet.h"
#include <iostream>
#include <fstream>
#include <string>

class EnemyBulletManager;

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

    bool horizontal;
    float hspeed;
    int min_x;
    int max_x;
    bool move_right;

    Enemy(float x, float start_y, float w, float h, float speed, EnemyType type, SDL_Renderer* renderer);
    ~Enemy();

    Enemy(Enemy&& other) noexcept;
    Enemy& operator=(Enemy&& other) noexcept;
    Enemy(const Enemy&) = delete;
    Enemy& operator=(const Enemy&) = delete;
    EnemyType parseEnemyType(const std::string& typeStr);


    void setHorizontalBounds(int minX, int maxX);
    void setHorizontalMovement(bool enabled, float speed, int minX, int maxX);
    void update(float dt);
    void draw(SDL_Renderer* renderer) const;
    void takeDamage(int amount);
    bool loadEnemiesFromFile(const char* filename);

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
    int screen_height;

    EnemyBulletManager* bullet_manager;


public:
    EnemyManager();

    void setupEnemies(SDL_Renderer* renderer, int play_x, int play_width, int screen_h);
    void update(float dt);
    void draw();
    void reset();
    bool allDestroyed() const;

    void setBulletManager(EnemyBulletManager* manager);
    void shootFromRandomEnemy();

    std::vector<Enemy>& getEnemies();
};