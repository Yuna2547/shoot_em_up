#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>

#define MAX_BULLETS 100

typedef struct {
    SDL_FRect rect;
    float speed;
    bool active;
} Bullet;

typedef struct {
    Bullet bullets[MAX_BULLETS];
    int count;
    float shoot_cooldown;
    float cooldown_timer;
} BulletManager;

// Initialize bullet manager
void BulletManager_Init(BulletManager* manager, float cooldown);

// Update cooldown timer
void BulletManager_Update(BulletManager* manager, float dt);

// Attempt to shoot a bullet
void BulletManager_Shoot(BulletManager* manager, float x, float y);

// Update all active bullets
void BulletManager_UpdateBullets(BulletManager* manager, float dt, int screen_height);

// Draw all active bullets
void BulletManager_Draw(BulletManager* manager, SDL_Renderer* renderer);
