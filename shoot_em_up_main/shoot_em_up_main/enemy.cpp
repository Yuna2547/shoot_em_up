#include "enemy.h"

// ---------------- Enemy ----------------
Enemy::Enemy(float x, float start_y, float target_y, float w, float h, float speed, EnemyType type, SDL_Renderer* renderer)
    : target_y(target_y), speed(speed), type(type), in_position(false), has_collided(false),
    health(10), max_health(10), sprite(nullptr) {
    rect.x = x;
    rect.y = start_y;
    rect.w = w;
    rect.h = h;

    // Charger le sprite correspondant au type d'ennemi
    const char* sprite_path = nullptr;
    switch (type) {
    case EnemyType::tomato:
        sprite_path = "assets/tomato.png";
        break;
    case EnemyType::brocolie:
        sprite_path = "assets/brocolie.png";
        break;
    case EnemyType::carrot:
        sprite_path = "assets/carrot.png";
        break;
    }

    if (renderer && sprite_path) {
        sprite = new Sprite(renderer, sprite_path);
        if (!sprite->IsValid()) {
            delete sprite;
            sprite = nullptr;
        }
    }
}

Enemy::~Enemy() {
    if (sprite) {
        delete sprite;
        sprite = nullptr;
    }
}

// Move constructor
Enemy::Enemy(Enemy&& other) noexcept
    : rect(other.rect), target_y(other.target_y), speed(other.speed),
    type(other.type), in_position(other.in_position),
    has_collided(other.has_collided), sprite(other.sprite),
    health(other.health), max_health(other.max_health) {
    other.sprite = nullptr;
}

// Move assignment
Enemy& Enemy::operator=(Enemy&& other) noexcept {
    if (this != &other) {
        if (sprite) delete sprite;

        rect = other.rect;
        target_y = other.target_y;
        speed = other.speed;
        type = other.type;
        in_position = other.in_position;
        has_collided = other.has_collided;
        sprite = other.sprite;
        health = other.health;
        max_health = other.max_health;

        other.sprite = nullptr;
    }
    return *this;
}

void Enemy::update(float dt) {
    if (!in_position) {
        rect.y += speed * dt / 1.5f;
    }
}

void Enemy::draw(SDL_Renderer* renderer) const {
    if (!isAlive()) return;

    // Draw the sprite first
    if (sprite && sprite->IsValid()) {
        SDL_FRect dstRect = rect;
        sprite->Draw(renderer, &dstRect);
    }

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
    : spawn_timer(0.0f), next_enemy_index(0), all_spawned(false), renderer(nullptr) {
}

void EnemyManager::setupEnemies(SDL_Renderer* renderer) {
    this->renderer = renderer;
    enemies.clear();

    // First row - 2 red enemies (will be activated first)
    enemies.emplace_back(250.0f, -100.0f, 50.0f, 70.0f, 70.0f, 150.0f, EnemyType::tomato, renderer);
    enemies.emplace_back(510.0f, -100.0f, 50.0f, 70.0f, 70.0f, 150.0f, EnemyType::tomato, renderer);

    // Second row - 2 yellow enemies
    enemies.emplace_back(250.0f, -100.0f, 120.0f, 70.0f, 70.0f, 150.0f, EnemyType::brocolie, renderer);
    enemies.emplace_back(510.0f, -100.0f, 120.0f, 70.0f, 70.0f, 150.0f, EnemyType::brocolie, renderer);

    // Third row - 2 blue enemies
    enemies.emplace_back(250.0f, -100.0f, 190.0f, 70.0f, 70.0f, 150.0f, EnemyType::carrot, renderer);
    enemies.emplace_back(510.0f, -100.0f, 190.0f, 70.0f, 70.0f, 150.0f, EnemyType::carrot, renderer);

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
    for (size_t i = 0; i < next_enemy_index && i < enemies.size(); i++) {
        enemies[i].update(dt);
    }
}

void EnemyManager::draw(SDL_Renderer* renderer) {
    // Draw only spawned and alive enemies
    for (size_t i = 0; i < next_enemy_index && i < enemies.size(); i++) {
        enemies[i].draw(renderer);
    }
}

void EnemyManager::reset() {
    setupEnemies(renderer);
}

bool EnemyManager::allDestroyed() const {
    if (!all_spawned) return false;

    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            return false;
        }
    }
    return true;
}