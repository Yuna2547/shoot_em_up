#include "bullet.h"
#include <SDL3/SDL.h>

//player's bullet
Bullet::Bullet() {      //parameters
    rect = { 0, 0, 5.0f, 15.0f };
    speed = 1000.0f;
    active = false;
}

void Bullet::update(float dt) {     //update for it to gu upward
    if (active) {
        rect.y -= speed * dt;

        if (rect.y + rect.h < 0.0f) 
            active = false;
    }
}

void Bullet::draw(SDL_Renderer* renderer) const {       //draws the bullet
    if (!renderer || !active) 
        return;

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderRect(renderer, &rect);
}

void Bullet::deactivate() {       //disable the bullet
    active = false;
}

const SDL_FRect& Bullet::getRect() const {      //gets the rect of the bullet
    return rect;
}

BulletManager::BulletManager(int maxBullets, float cooldown) 
    : max_bullets(maxBullets), shoot_cooldown(cooldown), cooldown_timer(0.0f){     //manages the bullets with cooldown{
	bullets.resize(max_bullets);
}

void BulletManager::update(float dt) {
    if (cooldown_timer > 0.0f) {
        cooldown_timer -= dt;
        if (cooldown_timer < 0.0f) 
            cooldown_timer = 0.0f;
    }
}

void BulletManager::shoot(float x, float y) {       //attack of the player when shooting a bullet
    if (cooldown_timer > 0.0f) 
        return;

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

void BulletManager::updateBullets(float dt) {       //update the bullets while still activated
    for (auto& bullet : bullets) {
        bullet.update(dt);
    }
}

void BulletManager::draw(SDL_Renderer* renderer) const {        //draws the bullets with it's color
    if (!renderer) 
        return;

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // yellow 

    for (const auto& bullet : bullets) {
        bullet.draw(renderer);
    }
}

void BulletManager::reset() {       //when reseted, delete bullet
    cooldown_timer = 0.0f;
    for (auto& bullet : bullets) {
        bullet.deactivate();
    }
}



std::vector<Bullet>& BulletManager::getBullets() {
    return bullets;
}


//enemy's bullets
EnemyBullet::EnemyBullet() {        //parameters
    rect = { 0, 0, 15.0f, 15.f };
    speed = 500.0f;
    active = false;

}

void EnemyBullet::update(float dt, int screen_height) {     //update for the bullet to move
    if (active) {
        rect.y += speed * dt; 

        if (rect.y > screen_height) 
            active = false;
    }
}

void EnemyBullet::draw(SDL_Renderer* renderer) const {      //draw the bullet for it to be neon green round bullets
    if (!renderer || !active) 
        return;
    
    auto centerX = static_cast<int>(rect.x + rect.w / 2.0f);
    auto centerY = static_cast<int>(rect.y + rect.h / 2.0f);
    auto radius = static_cast<int>(rect.w / 2.0f);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) 
                SDL_RenderPoint(renderer, centerX + x, centerY + y);
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
    for (int angle = 0; angle < 360; angle += 5) {
		float pi = 3.14159f;
        float rad = angle * pi / 180.0f;
        int x = centerX + static_cast<int>(radius * cos(rad));
        int y = centerY + static_cast<int>(radius * sin(rad));
        SDL_RenderPoint(renderer, x, y);
    }
}

void EnemyBullet::deactivate() {        //disable enemy's bullets
    active = false;
}


const SDL_FRect& EnemyBullet::getRect() const {
    return rect;
}



EnemyBulletManager::EnemyBulletManager(int maxBullets, float cooldown) 
    : max_bullets(maxBullets), shoot_cooldown(cooldown), cooldown_timer(0.0f){     //manages the cooldown and shooting system
	bullets.resize(max_bullets);
}

void EnemyBulletManager::update(float dt) {     //update the bullets
    if (cooldown_timer > 0.0f) {
        cooldown_timer -= dt;
        if (cooldown_timer < 0.0f) 
            cooldown_timer = 0.0f;
    }
}

bool EnemyBulletManager::canShoot() const {     //verify if shoot is available
    return cooldown_timer <= 0.0f;
}

void EnemyBulletManager::shoot(float x, float y) {      //enemy's attack
    if (cooldown_timer > 0.0f) 
        return;

    for (auto& bullet : bullets) {
        if (!bullet.active) {
            bullet.rect.x = x - bullet.rect.w / 2.0f;
            bullet.rect.y = y;
            bullet.active = true;

            cooldown_timer = shoot_cooldown;
            break;
        }
    }
}

void EnemyBulletManager::updateBullets(float dt, int screen_height) {
    for (auto& bullet : bullets) 
        bullet.update(dt, screen_height);
}

void EnemyBulletManager::draw(SDL_Renderer* renderer) {
    if (!renderer) 
        return;

    for (const auto& bullet : bullets) {
        bullet.draw(renderer);
    }
}

void EnemyBulletManager::reset() {
    for (auto& bullet : bullets) {
        bullet.deactivate();
    }
}

std::vector<EnemyBullet>& EnemyBulletManager::getBullets() {
    return bullets;
}