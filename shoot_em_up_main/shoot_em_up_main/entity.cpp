#include "entity.h"
#include "Sprite.h"
#include "enemy.h"

Entity::Entity()
    : speed(0), screen_width(0), screen_height(0), sprite(nullptr),
    health(20), max_health(20), invulnerable_timer(0.0f), offset_x(0)
{
    rect.x = rect.y = rect.w = rect.h = 0.0f;
}

Entity::Entity(float x, float y, float w, float h, float speed_, SDL_Renderer* renderer)
    : speed(speed_), screen_width(800), screen_height(600), sprite(nullptr),
    health(20), max_health(20), invulnerable_timer(0.0f), offset_x(0)
{
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    if (renderer) {
        sprite = new Sprite(renderer, "assets/player.png");
        if (!sprite->IsValid()) {
            delete sprite;
            sprite = nullptr;
        }
    }
}

Entity::~Entity() {
    delete sprite;
    sprite = nullptr;
}

void Entity::Init(float x, float y, float w, float h, float speed_, SDL_Renderer* renderer) {
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    speed = speed_;
    screen_width = 800;
    screen_height = 600;
    offset_x = 0;
    health = 10;
    max_health = 10;
    invulnerable_timer = 0.0f;

    delete sprite;
    sprite = nullptr;

    if (renderer) {
        sprite = new Sprite(renderer, "assets/player.png");
        if (!sprite->IsValid()) {
            delete sprite;
            sprite = nullptr;
        }
    }
}

void Entity::update(const bool* keys, float dt) {
    if (!keys) return;

  
    if (invulnerable_timer > 0.0f) {
        invulnerable_timer -= dt;
        if (invulnerable_timer < 0.0f) 
            invulnerable_timer = 0.0f;
    }

   
    if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) 
        rect.y -= speed * dt + 1;
    if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) 
        rect.y += speed * dt + 1;
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) 
        rect.x -= speed * dt + 1;
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) 
        rect.x += speed * dt + 1;

    if (rect.x < static_cast<float>(offset_x)) 
        rect.x = static_cast<float>(offset_x);
    if (rect.x + rect.w > static_cast<float>(offset_x + screen_width))
        rect.x = static_cast<float>(offset_x + screen_width) - rect.w;
    if (rect.y < 0.0f) 
        rect.y = 0.0f;
    if (rect.y + rect.h > static_cast<float>(screen_height))
        rect.y = static_cast<float>(screen_height) - rect.h;
    
}

void Entity::setScreenBounds(int width, int height) {
    screen_width = width;
    screen_height = height;
}

void Entity::setOffsetX(int offset) {
    offset_x = offset;
}

void Entity::draw(SDL_Renderer* renderer) const {
    if (!renderer) return;

    if (sprite && sprite->IsValid()) {
        SDL_FRect dstRect = rect;

        
        if (isInvulnerable() && ((int)(invulnerable_timer * 10) % 2 == 0))
            SDL_SetTextureAlphaMod(sprite->GetTexture(), 128);
        else 
            SDL_SetTextureAlphaMod(sprite->GetTexture(), 255);

        sprite->Draw(renderer, &dstRect);
        SDL_SetTextureAlphaMod(sprite->GetTexture(), 255);
    }
    else {
        if (isInvulnerable() && ((int)(invulnerable_timer * 10) % 2 == 0)) 
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        else 
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

    float bar_width = rect.w;
    float bar_height = 8.0f;
    float bar_x = rect.x;
    float bar_y = rect.y - 20.0f;

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

void Entity::takeDamage(int amount) {
    if (!isInvulnerable()) {
        health -= amount;
        if (health < 0) health = 0;
        invulnerable_timer = 0.5f;
    }
}

void Entity::resetPosition(float x, float y) {
    rect.x = x;
    rect.y = y;
}

void Entity::resetHealth() {
    health = max_health;
    invulnerable_timer = 0.0f;
}

const SDL_FRect& Entity::getRect() const {
    return rect;
}

int Entity::getHealth() const {
    return health;
}

bool Entity::isInvulnerable() const {
    return invulnerable_timer > 0.0f;
}