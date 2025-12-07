#include "enemy.h"

// ---------------- Enemy ----------------
Enemy::Enemy(float x, float start_y, float target_y, float w, float h, float speed, EnemyType type)
    : target_y(target_y), speed(speed), type(type), in_position(false), has_collided(false) {
    rect.x = x;
    rect.y = start_y;
    rect.w = w;
    rect.h = h;
}

void Enemy::update(float dt) {
    if (!in_position) {
        rect.y += speed * dt;
    }
}

void Enemy::draw(SDL_Renderer* renderer) const {
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
    // Draw only spawned enemies
    for (int i = 0; i < next_enemy_index && i < enemies.size(); i++) {
        enemies[i].draw(renderer);
    }
}

void EnemyManager::reset() {
    setupEnemies();
}