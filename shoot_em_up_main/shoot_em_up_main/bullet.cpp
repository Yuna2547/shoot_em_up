#include "bullet.h"
#include <SDL3/SDL.h>

// ---------------- Bullet ----------------
Bullet::Bullet() {
    rect = { 0, 0, 5.0f, 15.0f };
    speed = 400.0f;
    active = false;
}

void Bullet::update(float dt, int screen_height) {
    if (active) {
        rect.y -= speed * dt;

        // Deactivate if off screen
        if (rect.y + rect.h < 0.0f) {
            active = false;
        }
    }
}

void Bullet::draw(SDL_Renderer* renderer) const {
    if (!renderer || !active) return;

    // SDL3: Fill with SDL_FRect directly
    SDL_RenderFillRect(renderer, &rect);

    // Optional: outline for clarity
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderRect(renderer, &rect);
}

void Bullet::deactivate() {
    active = false;
}

const SDL_FRect& Bullet::getRect() const {
    return rect;
}

// ---------------- BulletManager ----------------
BulletManager::BulletManager(int maxBullets, float cooldown)
    : max_bullets(maxBullets), shoot_cooldown(cooldown), cooldown_timer(0.0f) {
    bullets.resize(max_bullets);
}

void BulletManager::update(float dt) {
    if (cooldown_timer > 0.0f) {
        cooldown_timer -= dt;
        if (cooldown_timer < 0.0f) {
            cooldown_timer = 0.0f;
        }
    }
}

void BulletManager::shoot(float x, float y) {
    if (cooldown_timer > 0.0f) return;

    for (auto& bullet : bullets) {
        if (!bullet.active) {
            bullet.rect.x = x;
            bullet.rect.y = y;
            bullet.active = true;

            cooldown_timer = shoot_cooldown;
            break;
        }
    }
}

void BulletManager::updateBullets(float dt, int screen_height) {
    for (auto& bullet : bullets) {
        bullet.update(dt, screen_height);
    }
}

void BulletManager::draw(SDL_Renderer* renderer) {
    if (!renderer) return;

    // Set bullet color once
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow bullets

    for (const auto& bullet : bullets) {
        bullet.draw(renderer);
    }
}

std::vector<Bullet>& BulletManager::getBullets() {
    return bullets;
}