#include "entity.h"
#include "Sprite.h"

Entity::Entity()
 : speed(0), screen_width(0), screen_height(0), sprite(nullptr)
{
 rect.x = rect.y = rect.w = rect.h =0.0f;
}

Entity::Entity(float x, float y, float w, float h, float speed_, SDL_Renderer* renderer)
 : speed(speed_), screen_width(800), screen_height(600), sprite(nullptr)
{
 rect.x = x;
 rect.y = y;
 rect.w = w;
 rect.h = h;
 if (renderer) {
 sprite = new Sprite(renderer, "player.png");
 if (!sprite->IsValid()) { delete sprite; sprite = nullptr; }
 }
}

Entity::~Entity() {
 if (sprite) { delete sprite; sprite = nullptr; }
}

void Entity::Init(float x, float y, float w, float h, float speed_, SDL_Renderer* renderer) {
 rect.x = x;
 rect.y = y;
 rect.w = w;
 rect.h = h;
 speed = speed_;
 screen_width =800;
 screen_height =600;
 if (sprite) { delete sprite; sprite = nullptr; }
 if (renderer) {
 sprite = new Sprite(renderer, "player.png");
 if (!sprite->IsValid()) { delete sprite; sprite = nullptr; }
 }
}

void Entity::Update(const bool* keys, float dt) {
 if (!keys) return;
 // Vertical controls
 if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) {
 rect.y -= speed * dt;
 }
 if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) {
 rect.y += speed * dt;
 }

 // Horizontal controls
 if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
 rect.x -= speed * dt;
 }
 if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
 rect.x += speed * dt;
 }

 // Screen's limits
 if (rect.x <0) rect.x =0;
 if (rect.y <0) rect.y =0;
 if (rect.x + rect.w > screen_width) rect.x = screen_width - rect.w;
 if (rect.y + rect.h > screen_height) rect.y = screen_height - rect.h;
}

void Entity::SetScreenBounds(int width, int height) {
 screen_width = width;
 screen_height = height;
}

void Entity::Draw(SDL_Renderer* renderer) {
 if (sprite) sprite->Draw(renderer, &rect);
}

void Entity::takeDamage(int amount) {
    if (!isInvulnerable()) {
        health -= amount;
        if (health < 0) health = 0;

        // Set invulnerability period (1 second)
        invulnerable_timer = 1.0f;
    }
}