#include "bullet.h"

void BulletManager_Init(BulletManager* manager, float cooldown) {
    manager->count = 0;

    // Initialize all bullets as inactive
    for (int i = 0; i < MAX_BULLETS; i++) {
        manager->bullets[i].active = false;
    }
}

void BulletManager_Shoot(BulletManager* manager, float x, float y) {

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