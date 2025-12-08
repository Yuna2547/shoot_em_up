#include "enemy.h"

// ---------------- Enemy ----------------
Enemy::Enemy(float x, float start_y, float target_y, float w, float h, float speed, EnemyType type)
    : target_y(target_y), speed(speed), type(type), in_position(false), has_collided(false),
    health(10), max_health(10) {
    rect.x = x;
    rect.y = start_y;
    rect.w = w;
    rect.h = h;
}

void Enemy::update(float dt) {
    if (!in_position) {
        rect.y += speed * dt / 1.5;
    }
}

void Enemy::draw(SDL_Renderer* renderer) const {
    if (!isAlive()) return;

    // Set color based on enemy type
    switch (type) {
    case EnemyType::RED:
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        break;
    case EnemyType::YELLOW:
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        break;
    case EnemyType::BLUE:
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        break;
    }
    SDL_RenderFillRect(renderer, &rect);

    // Draw health bar above the enemy
    float bar_width = rect.w;
    float bar_height = 6.0f;
    float bar_x = rect.x;
    float bar_y = rect.y - 10.0f;

    // Background (dark red)
    SDL_FRect bg_rect = { bar_x, bar_y, bar_width, bar_height };
    SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bg_rect);

    // Health (green)
    float health_width = (bar_width * health) / max_health;
    SDL_FRect health_rect = { bar_x, bar_y, health_width, bar_height };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &health_rect);

    // Border (white)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderRect(renderer, &bg_rect);
}

void Enemy::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

// ---------------- EnemyManager ----------------
EnemyManager::EnemyManager()
    : spawn_timer(0.0f), next_enemy_index(0), all_spawned(false) {
}

void EnemyManager::setupEnemies() {
    enemies.clear();

    // First row - 2 red enemies (will be activated first)
    enemies.emplace_back(250.0f, -100.0f, 50.0f, 40.0f, 40.0f, 150.0f, EnemyType::RED);
    enemies.emplace_back(510.0f, -100.0f, 50.0f, 40.0f, 40.0f, 150.0f, EnemyType::RED);

    // Second row - 2 yellow enemies
    enemies.emplace_back(250.0f, -100.0f, 120.0f, 40.0f, 40.0f, 150.0f, EnemyType::YELLOW);
    enemies.emplace_back(510.0f, -100.0f, 120.0f, 40.0f, 40.0f, 150.0f, EnemyType::YELLOW);

    // Third row - 2 blue enemies
    enemies.emplace_back(250.0f, -100.0f, 190.0f, 40.0f, 40.0f, 150.0f, EnemyType::BLUE);
    enemies.emplace_back(510.0f, -100.0f, 190.0f, 40.0f, 40.0f, 150.0f, EnemyType::BLUE);

    next_enemy_index = 0;
    all_spawned = false;
    spawn_timer = 0.0f;
}

void EnemyManager::update(float dt) {
    // Spawn enemies one by one with delay
    if (!all_spawned) {
        spawn_timer += dt;

        // Spawn next enemy every 0.5 seconds
        if (spawn_timer >= 0.5f && next_enemy_index < enemies.size()) {
            // "Activate" the enemy by moving it from off-screen
            next_enemy_index++;
            spawn_timer = 0.0f;

            if (next_enemy_index >= enemies.size()) {
                all_spawned = true;
            }
        }
    }

    // Update only spawned enemies
    for (int i = 0; i < next_enemy_index && i < enemies.size(); i++) {
        enemies[i].update(dt);
    }
}

void EnemyManager::draw(SDL_Renderer* renderer) {
    // Draw only spawned and alive enemies
    for (int i = 0; i < next_enemy_index && i < enemies.size(); i++) {
        enemies[i].draw(renderer);
    }
}

void EnemyManager::reset() {
    setupEnemies();
}