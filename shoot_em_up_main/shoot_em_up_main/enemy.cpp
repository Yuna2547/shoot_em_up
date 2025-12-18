#include "enemy.h"
#include <iostream>
#include <fstream>
#include <string>

Enemy::Enemy(float x, float start_y, float w, float h, float speed, EnemyType type, SDL_Renderer* renderer)
    : speed(speed), type(type), has_collided(false), health(10), max_health(10), sprite(nullptr), horizontal(false), hspeed(0.0f), min_x(0), max_x(0), move_right(true) {

    rect.x = x;
    rect.y = start_y;
    rect.w = w;
    rect.h = h;

    const char* sprite_path = nullptr;
    switch (type) {
    case EnemyType::tomato: sprite_path = "assets/tomato.png"; break;
    case EnemyType::broccoli: sprite_path = "assets/brocolie.png"; break;
    case EnemyType::carrot: sprite_path = "assets/carrot.png"; break;
    }

    if (renderer && sprite_path) {
        sprite = new Sprite(renderer, sprite_path);
        if (!sprite->IsValid()) {
            delete sprite;
            sprite = nullptr;
        }
    }

}

bool Enemy::isOffScreen(int screen_height) const {
    return rect.y > screen_height; 
}

Enemy::~Enemy() {
    delete sprite;
    sprite = nullptr;
}

Enemy::Enemy(Enemy&& other) noexcept
    : rect(other.rect), speed(other.speed), type(other.type), has_collided(other.has_collided), sprite(other.sprite), health(other.health), max_health(other.max_health),
    horizontal(other.horizontal), hspeed(other.hspeed), min_x(other.min_x), max_x(other.max_x), move_right(other.move_right) {
    other.sprite = nullptr;    // Prevent the moved-from object from deleting the sprite
}

Enemy& Enemy::operator=(Enemy&& other) noexcept {
    if (this != &other) {
        delete sprite;

        rect = other.rect;
        speed = other.speed;
        type = other.type;
        has_collided = other.has_collided;
        sprite = other.sprite;
        health = other.health;
        max_health = other.max_health;
        horizontal = other.horizontal;
        hspeed = other.hspeed;
        min_x = other.min_x;
        max_x = other.max_x;
        move_right = other.move_right;

        other.sprite = nullptr;
    }
    return *this;
}

EnemyType Enemy::parseEnemyType(const std::string& typeStr) {
    if (typeStr == "tomato") 
        return EnemyType::tomato;
    if (typeStr == "broccoli") 
        return EnemyType::broccoli;
    if (typeStr == "carrot") 
        return EnemyType::carrot;
    return EnemyType();
}

void Enemy::setHorizontalBounds(int minX, int maxX) {
    min_x = minX;
    max_x = maxX;
    if (rect.x < minX) 
        rect.x = static_cast<float>(min_x);
    if (rect.x + rect.w > maxX) 
        rect.x = static_cast<float>(max_x - rect.w);
}

void Enemy::setHorizontalMovement(bool enabled, float speed, int minX, int maxX) {
    horizontal = enabled;
    hspeed = speed;
    min_x = minX;
    max_x = maxX;
}

void Enemy::update(float dt) {
    if (!isAlive()) 
        return; 
    rect.y += speed * dt;

    if (horizontal) {
        if (move_right) 
            rect.x += hspeed * dt;
        else            
            rect.x -= hspeed * dt;

        if (rect.x <= min_x) {
            rect.x = min_x;
            move_right = true;
        }
        else if (rect.x + rect.w >= max_x) {
            rect.x = max_x - rect.w;
            move_right = false;
        }
    }

}

void Enemy::draw(SDL_Renderer* renderer) const {
    if (!isAlive()) 
        return; 

    if (sprite && sprite->IsValid()) {
        SDL_FRect dstRect = rect;
        sprite->Draw(renderer, &dstRect);
    }

    float bar_width = rect.w;
    float bar_height = 6.0f;
    float bar_x = rect.x;
    float bar_y = rect.y - 10.0f;

    SDL_FRect bg_rect = { bar_x, bar_y, bar_width, bar_height };
    SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bg_rect);

    float health_width = (bar_width * health) / max_health;
    SDL_FRect health_rect = { bar_x, bar_y, health_width, bar_height };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &health_rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderRect(renderer, &bg_rect);
}

void Enemy::takeDamage(int amount) {
    health -= amount;
    if (health < 0) 
        health = 0;
}

bool Enemy::loadEnemiesFromFile(const char* filename) const{
    std::ifstream File(filename);
    std::string lineFromFile;

    return false;
}

bool Enemy::isAlive() const {
    return health > 0;
}

bool Enemy::hasCollided() const {
    return has_collided;
}

void Enemy::setCollided() {
    has_collided = true;
}

const SDL_FRect& Enemy::getRect() const {
    return rect;
}



EnemyManager::EnemyManager()
    : spawn_timer(0.0f), next_enemy_index(0), all_spawned(false), renderer(nullptr), play_area_x(0), play_area_width(0), screen_height(0), bullet_manager(nullptr) {
}

void EnemyManager::setupEnemies(SDL_Renderer* renderer, int play_x, int play_width, int screen_h) {
    if (!renderer || play_width <= 0) 
        return;

    this->renderer = renderer;
    this->play_area_x = play_x;
    this->play_area_width = play_width;
    this->screen_height = screen_h;
    enemies.clear(); 

    auto addEnemy = [&](float rel_x, float start_y, float w, float h, float speed, EnemyType type) {
            
            float world_x = play_x + rel_x;
            enemies.emplace_back(world_x, start_y, w, h, speed, type, renderer);
            Enemy& e = enemies.back();
            if (type == EnemyType::tomato)
                e.setHorizontalMovement(true, 120.0f, play_x, play_x + play_width);
    };
    
    std::ifstream myfile("setUpEnemy.txt");

    std::string currentLine;
    if (myfile.is_open()) {
        while (myfile.good()) {

            std::getline(myfile, currentLine);
            if (currentLine.empty())
                continue;
            float relX = std::stof(currentLine);
            std::getline(myfile, currentLine);
            float start_y = std::stof(currentLine);
            std::getline(myfile, currentLine);
            float w = std::stof(currentLine);
            std::getline(myfile, currentLine);
            float h = std::stof(currentLine);
            std::getline(myfile, currentLine);
            float speed = std::stof(currentLine);
            std::getline(myfile, currentLine);
            EnemyType enemy = Enemy::parseEnemyType(currentLine);
            addEnemy(relX, start_y, w, h, speed, enemy);
          


            
        }

    }
    next_enemy_index = enemies.size();
    all_spawned = true;
    spawn_timer = 0.5f;
}

void EnemyManager::update(float dt) {
    for (size_t i = 0; i < next_enemy_index && i < enemies.size(); i++) {
        enemies[i].update(dt);
    }
}

void EnemyManager::draw() {
    if (!renderer) 
        return;
    for (size_t i = 0; i < next_enemy_index && i < enemies.size(); i++) {
        enemies[i].draw(renderer);
    }

}


void EnemyManager::reset() {
    if (renderer) 
        setupEnemies(renderer, play_area_x, play_area_width, screen_height);
}


bool EnemyManager::allDestroyed() const {
    if (!all_spawned) return false; 
    for (const auto& enemy : enemies) {
        if (enemy.isAlive() && !enemy.isOffScreen(screen_height)) return false;
    }
    return true;
}

void EnemyManager::setBulletManager(EnemyBulletManager* manager) {
    bullet_manager = manager;
}

void EnemyManager::shootFromRandomEnemy() {
    if (!bullet_manager) return;

    std::vector<Enemy*> alive_enemies;
    for (size_t i = 0; i < next_enemy_index && i < enemies.size(); i++) {
        if (enemies[i].isAlive() && enemies[i].rect.y >= 0) 
            alive_enemies.push_back(&enemies[i]);
    }

    if (alive_enemies.empty()) 
        return;

    int random_index = rand() % alive_enemies.size();
    Enemy* shooter = alive_enemies[random_index];

    float bullet_x = shooter->rect.x + shooter->rect.w / 2.0f;
    float bullet_y = shooter->rect.y + shooter->rect.h;

    bullet_manager->shoot(bullet_x, bullet_y);
}


 
std::vector<Enemy>& EnemyManager::getEnemies() {
    return enemies;
}