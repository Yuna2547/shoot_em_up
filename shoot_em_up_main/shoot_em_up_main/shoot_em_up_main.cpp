#include <SDL3/SDL.h>
#include "entity.h"
#include "bullet.h"
#include "Sprite.h"
#include "enemy.h"
#include <SDL3_image/SDL_image.h>

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

    SDL_Window* window = SDL_CreateWindow("Shoot 'Em Up", 0, 0,
        SDL_WINDOW_FULLSCREEN);

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

    // Query actual screen size
    int screen_w = 0, screen_h = 0;
    SDL_GetWindowSize(window, &screen_w, &screen_h);

    // Load background safely (expects a PNG at assets/background.png)
    SDL_Texture* bg_texture = NULL;
    SDL_Surface* bg_surface = IMG_Load("assets/background.png");
    if (bg_surface) {
        bg_texture = SDL_CreateTextureFromSurface(renderer, bg_surface);
        SDL_DestroySurface(bg_surface);
        if (!bg_texture) {
            SDL_Log("Warning: could not create background texture: %s", SDL_GetError());
        }
    }
    else {
        SDL_Log("Warning: could not load background image: %s", SDL_GetError());
    }

    // Entity creation
    Entity player(screen_w / 2.0f, screen_h / 2.0f, 100.0f, 100.0f, 200.0f, renderer);
    player.setScreenBounds(screen_w, screen_h);

    // Bullet manager
    BulletManager bulletManager(100, 0.1f);

    // Enemy manager with scrolling spawn
    EnemyManager enemyManager;
    enemyManager.setupEnemies(renderer);

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
            // Press R to reset enemies, ESC to quit
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_R) {
                    enemyManager.reset();
                }
                if (event.key.key == SDLK_ESCAPE) {
                    running = false;
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

        // CORRIGÉ: Clear d'abord, puis dessiner
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw background
        if (bg_texture) {
            SDL_RenderTexture(renderer, bg_texture, NULL, NULL);
        }

        enemyManager.draw(renderer);
        player.draw(renderer);
        bulletManager.draw(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);

    }

    // Cleanup
    if (bg_texture) SDL_DestroyTexture(bg_texture);
    // player destructor will run automatically
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}