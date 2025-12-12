#include "enemy.h"

// ---------------- Enemy ----------------
Enemy::Enemy(float x, float start_y, float w, float h, float speed,
    EnemyType type, SDL_Renderer* renderer)
    : speed(speed), type(type), has_collided(false),
    health(10), max_health(10), sprite(nullptr),
    horizontal(false), hspeed(0.0f), move_right(true),
    min_x(0), max_x(0) {

    // Set up the enemy's rectangle (position and size)
    rect.x = x;
    rect.y = start_y;
    rect.w = w;
    rect.h = h;

    // Select the appropriate sprite image based on enemy type
    const char* sprite_path = nullptr;
    switch (type) {
    case EnemyType::tomato:   sprite_path = "assets/tomato.png"; break;
    case EnemyType::broccoli: sprite_path = "assets/brocolie.png"; break;
    case EnemyType::carrot:   sprite_path = "assets/carrot.png"; break;
    }

    // Load the sprite from file if renderer and path are valid
    if (renderer && sprite_path) {
        sprite = new Sprite(renderer, sprite_path);
        // If sprite loading failed, clean up and set to nullptr
        if (!sprite->IsValid()) {
            delete sprite;
            sprite = nullptr;
        }
    }

}

bool Enemy::isOffScreen(int screen_height) const {
    return rect.y > screen_height;  // Enemy has fallen below the screen
}

Enemy::~Enemy() {
    delete sprite;
    sprite = nullptr;
}

// Move constructor
Enemy::Enemy(Enemy&& other) noexcept
    : rect(other.rect), speed(other.speed), type(other.type),
    has_collided(other.has_collided), sprite(other.sprite),
    health(other.health), max_health(other.max_health),
    horizontal(other.horizontal), hspeed(other.hspeed),
    min_x(other.min_x), max_x(other.max_x), move_right(other.move_right) {
    // Prevent the moved-from object from deleting the sprite
    other.sprite = nullptr;
}

// Move assignment
Enemy& Enemy::operator=(Enemy&& other) noexcept {
    if (this != &other) {
        // Clean up our current sprite
        delete sprite;

        // Copy all data from the other enemy
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

        // Prevent the moved-from object from deleting the sprite
        other.sprite = nullptr;
    }
    return *this;
}

void Enemy::setHorizontalBounds(int minX, int maxX) {
    min_x = minX;
    max_x = maxX;
    if (rect.x < minX) rect.x = static_cast<float>(min_x);
    if (rect.x + rect.w > maxX) rect.x = static_cast<float>(max_x - rect.w);
}

void Enemy::setHorizontalMovement(bool enabled, float speed, int minX, int maxX) {
    horizontal = enabled;
    hspeed = speed;
    min_x = minX;
    max_x = maxX;
}

void Enemy::update(float dt) {
    if (!isAlive()) return; // Dead enemies don't move

    // Always move downward (falling)
    rect.y += speed * dt;

    // Optional horizontal movement (e.g., for tomatoes that move side-to-side)
    if (horizontal) {
        // Move left or right based on current direction
        if (move_right) rect.x += hspeed * dt;
        else            rect.x -= hspeed * dt;

        // Bounce off boundaries by reversing direction
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
    if (!isAlive()) return; // Don't draw dead enemies

    // Draw the enemy sprite (tomato, broccoli, or carrot image)
    if (sprite && sprite->IsValid()) {
        SDL_FRect dstRect = rect;
        sprite->Draw(renderer, &dstRect);
    }

    // Health bar dimensions and position (above the enemy)
    float bar_width = rect.w;
    float bar_height = 6.0f;
    float bar_x = rect.x;
    float bar_y = rect.y - 10.0f;

    // Draw health bar background (dark red)
    SDL_FRect bg_rect = { bar_x, bar_y, bar_width, bar_height };
    SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bg_rect);

    // Draw current health (green bar that shrinks as health decreases)
    float health_width = (bar_width * health) / max_health;
    SDL_FRect health_rect = { bar_x, bar_y, health_width, bar_height };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &health_rect);

    // Draw health bar border (white outline)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderRect(renderer, &bg_rect);
}

void Enemy::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
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

// ---------------- EnemyManager ----------------
EnemyManager::EnemyManager()
    : spawn_timer(0.0f), next_enemy_index(0), all_spawned(false),
    renderer(nullptr), play_area_x(0), play_area_width(0) {
}

void EnemyManager::setupEnemies(SDL_Renderer* renderer, int play_x, int play_width) {
    if (!renderer || play_width <= 0) return;

    // Store renderer and play area info for later use
    this->renderer = renderer;
    this->play_area_x = play_x;
    this->play_area_width = play_width;
    enemies.clear(); // Remove any existing enemies

    // Helper lambda function to create and add enemies
    auto addEnemy = [&](float rel_x, float start_y, float w, float h,
        float speed, EnemyType type) {
            // Convert relative x position to world coordinates
            float world_x = play_x + rel_x;
            enemies.emplace_back(world_x, start_y, w, h, speed, type, renderer);
            Enemy& e = enemies.back();
            // Tomatoes get special horizontal movement
            if (type == EnemyType::tomato)
                e.setHorizontalMovement(true, 120.0f, play_x, play_x + play_width);
        };

    // Row 1: Tomatoes (move horizontally side-to-side)
    addEnemy(80.0f, -300.0f, 70.0f, 70.0f, 150.0f, EnemyType::tomato);
    addEnemy(play_width - 150.0f, -750.0f, 70.0f, 70.0f, 150.0f, EnemyType::tomato);
    addEnemy(-100.0f, -50.0f, 70.0f, 70.0f, 150.0f, EnemyType::tomato);
    addEnemy(play_width - 250.0f, -500.0f, 70.0f, 70.0f, 150.0f, EnemyType::tomato);

    // Row 2: Broccoli (fall straight down)
    addEnemy(100.0f, -200.0f, 70.0f, 70.0f, 150.0f, EnemyType::broccoli);
    addEnemy(play_width - 150.0f, -550.0f, 70.0f, 70.0f, 150.0f, EnemyType::broccoli);
    addEnemy(500.0f, -400.0f, 70.0f, 70.0f, 150.0f, EnemyType::broccoli);

    // Row 3: Carrots (fall straight down)
    addEnemy(200.0f, -300.0f, 70.0f, 70.0f, 150.0f, EnemyType::carrot);
    addEnemy(play_width - 550.0f, -750.0f, 70.0f, 70.0f, 150.0f, EnemyType::carrot);

    // All enemies are immediately active (no delayed spawning)
    next_enemy_index = enemies.size();
    all_spawned = true;
    spawn_timer = 0.0f;
}

//Updates all active enemies each frame
void EnemyManager::update(float dt) {
    // Update only enemies that have been spawned
    for (size_t i = 0; i < next_enemy_index && i < enemies.size(); i++) {
        enemies[i].update(dt);
    }
}

//Draws all active enemies on screen
void EnemyManager::draw() {
    if (!renderer) return;
    // Draw only enemies that have been spawned
    for (size_t i = 0; i < next_enemy_index && i < enemies.size(); i++) {
        enemies[i].draw(renderer);
    }
}


//Resets the level by recreating all enemies
void EnemyManager::reset() {
    if (renderer) {
        setupEnemies(renderer, play_area_x, play_area_width);
    }
}

// Checks if all enemies have been destroyed (win condition)

bool EnemyManager::allDestroyed() const {
    if (!all_spawned) return false; // Can't win if enemies aren't all spawned yet
    // Check if any enemy is still alive
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) return false;
    }
    return true; // All enemies are dead - player wins!
}

//Provides access to the enemy vector for collision detection
 
std::vector<Enemy>& EnemyManager::getEnemies() {
    return enemies;
}