#include <SDL3/SDL.h>
#include "entity.h"
#include "bullet.h"
#include "enemy.h"

// Collision detection helper
bool checkCollision(const SDL_FRect& a, const SDL_FRect& b) {
    return (a.x < b.x + b.w &&
        a.x + a.w > b.x &&
        a.y < b.y + b.h &&
        a.y + a.h > b.y);
}

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error SDL_Init: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Shoot 'Em Up", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("Error creating window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Error creating renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Player entity
    Entity player(400.0f, 500.0f, 50.0f, 50.0f, 200.0f);

    // Bullet manager
    BulletManager bulletManager(100, 0.1f);

    // Enemy manager with scrolling spawn
    EnemyManager enemyManager;
    enemyManager.setupEnemies();

    bool running = true;
    SDL_Event event;
    Uint64 last_time = SDL_GetTicks();

    while (running) {
        Uint64 current_time = SDL_GetTicks();
        float dt = (current_time - last_time) / 1000.0f;
        last_time = current_time;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            // Press R to reset enemies
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_R) {
                    enemyManager.reset();
                }
            }
        }

        const bool* keys = SDL_GetKeyboardState(NULL);
        player.update(keys, dt);

        bulletManager.update(dt);

        if (keys[SDL_SCANCODE_SPACE]) {
            float bullet_x = player.getRect().x + (player.getRect().w / 2.0f) - 2.5f;
            float bullet_y = player.getRect().y;
            bulletManager.shoot(bullet_x, bullet_y);
        }

        bulletManager.updateBullets(dt, 600);
        enemyManager.update(dt);

        // Check collision between bullets and enemies
        for (auto& bullet : bulletManager.getBullets()) {
            if (!bullet.active) continue;

            for (auto& enemy : enemyManager.getEnemies()) {
                if (!enemy.isAlive()) continue;

                if (checkCollision(bullet.getRect(), enemy.getRect())) {
                    enemy.takeDamage(2);
                    bullet.deactivate();
                    break; // Bullet can only hit one enemy
                }
            }
        }

        // Check collision between player and enemies
        const SDL_FRect& playerRect = player.getRect();
        for (auto& enemy : enemyManager.getEnemies()) {
            if (!enemy.hasCollided() && enemy.isAlive() && checkCollision(playerRect, enemy.getRect())) {
                player.takeDamage(1);
                enemy.setCollided();
            }
        }

        // Check if player is dead
        if (player.getHealth() <= 0) {
            SDL_Log("Game Over! Press R to restart.");
            // You can add game over logic here
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        enemyManager.draw(renderer);
        player.draw(renderer);
        bulletManager.draw(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}