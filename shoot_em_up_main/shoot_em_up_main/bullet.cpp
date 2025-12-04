#include "bullet.h"

void BulletManager_Init(BulletManager* manager, float cooldown) {
    manager->count = 0;
    manager->shoot_cooldown = cooldown;
    manager->cooldown_timer = 0.0f;

    // Initialize all bullets as inactive
    for (int i = 0; i < MAX_BULLETS; i++) {
        manager->bullets[i].active = false;
    }
}

void BulletManager_Update(BulletManager* manager, float dt) {
    // Update cooldown timer
    if (manager->cooldown_timer > 0.0f) {
        manager->cooldown_timer -= dt;
        if (manager->cooldown_timer < 0.0f) {
            manager->cooldown_timer = 0.0f;
        }
    }
}

void BulletManager_Shoot(BulletManager* manager, float x, float y) {
    // Check if we can shoot (cooldown expired)
    if (manager->cooldown_timer > 0.0f) {
        return;
    }

    // Find an inactive bullet slot
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!manager->bullets[i].active) {
            // Activate this bullet
            manager->bullets[i].rect.x = x;
            manager->bullets[i].rect.y = y;
            manager->bullets[i].rect.w = 5.0f;
            manager->bullets[i].rect.h = 15.0f;
            manager->bullets[i].speed = 400.0f;
            manager->bullets[i].active = true;

            // Reset cooldown timer
            manager->cooldown_timer = manager->shoot_cooldown;
            break;
        }
    }
}

void BulletManager_UpdateBullets(BulletManager* manager, float dt, int screen_height) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (manager->bullets[i].active) {
            // Move bullet upward
            manager->bullets[i].rect.y -= manager->bullets[i].speed * dt;

            // Deactivate if off screen
            if (manager->bullets[i].rect.y + manager->bullets[i].rect.h < 0) {
                manager->bullets[i].active = false;
            }
        }
    }
}

void BulletManager_Draw(BulletManager* manager, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow bullets

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (manager->bullets[i].active) {
            SDL_RenderFillRect(renderer, &manager->bullets[i].rect);
        }
    }
}