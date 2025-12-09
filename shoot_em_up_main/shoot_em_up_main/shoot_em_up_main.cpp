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

    SDL_Window* window = SDL_CreateWindow("Shoot 'Em Up", 800, 600,
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

    // Define play area (55% of screen width, centered)
    int play_width = static_cast<int>(screen_w * 0.55f);
    int play_x = (screen_w - play_width) / 2;

    SDL_Log("Screen: %dx%d, Play area: x=%d, width=%d", screen_w, screen_h, play_x, play_width);

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

    // Store initial player position for reset
    float initial_player_x = play_x + play_width / 2.0f - 50.0f;
    float initial_player_y = screen_h - 150.0f;

    // Create player entity centered in play area
    Entity player(initial_player_x, initial_player_y, 100.0f, 130.0f, 200.0f, renderer);
    player.setScreenBounds(play_width, screen_h);
    player.setOffsetX(play_x);

    // Bullet manager
    BulletManager bulletManager(100, 0.1f);

    // Enemy manager
    EnemyManager enemyManager;
    enemyManager.setupEnemies(renderer, play_x, play_width);

    // Game state
    bool running = true;
    bool game_over = false;
    bool victory = false;

    SDL_Event event;
    Uint64 last_time = SDL_GetTicks();

    while (running) {
        Uint64 current_time = SDL_GetTicks();
        float dt = (current_time - last_time) / 1000.0f;
        last_time = current_time;

        // Event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }

            if (event.type == SDL_EVENT_KEY_DOWN) {
                // Press R to reset game
                if (event.key.key == SDLK_R) {
                    SDL_Log("Resetting game...");

                    // Reset player
                    player.resetPosition(initial_player_x, initial_player_y);
                    player.resetHealth();

                    // Clear all bullets
                    bulletManager = BulletManager(100, 0.1f);

                    // Reset enemies
                    enemyManager.setupEnemies(renderer, play_x, play_width);

                    // Reset game state
                    game_over = false;
                    victory = false;

                    SDL_Log("Game reset complete!");
                }

                // Press ESC to quit
                if (event.key.key == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }

        // Only update game if not game over or victory
        if (!game_over && !victory) {
            // Player input and movement
            const bool* keys = SDL_GetKeyboardState(NULL);
            player.update(keys, dt);

            // Shooting
            if (keys[SDL_SCANCODE_SPACE]) {
                float bullet_x = player.getRect().x + (player.getRect().w / 2.0f) - 2.5f;
                float bullet_y = player.getRect().y;
                bulletManager.shoot(bullet_x, bullet_y);
            }

            // Update bullets and enemies
            bulletManager.update(dt);
            bulletManager.updateBullets(dt, screen_h);
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

            // Check win/lose conditions
            if (player.getHealth() <= 0) {
                game_over = true;
                SDL_Log("Game Over! Press R to restart.");
            }

            if (enemyManager.allDestroyed()) {
                victory = true;
                SDL_Log("Victory! All enemies destroyed! Press R to play again.");
            }
        }

        // ===== RENDERING =====

        // Clear screen first
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw background
        if (bg_texture) {
            SDL_RenderTexture(renderer, bg_texture, NULL, NULL);
        }

        // Draw game entities FIRST (on the play area)
        enemyManager.draw();
        player.draw(renderer);
        bulletManager.draw(renderer);

        // Draw black bars on sides LAST (to cover anything outside play area)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_FRect leftBar = { 0.0f, 0.0f, static_cast<float>(play_x), static_cast<float>(screen_h) };
        SDL_RenderFillRect(renderer, &leftBar);

        SDL_FRect rightBar = { static_cast<float>(play_x + play_width), 0.0f,
                               static_cast<float>(screen_w - play_x - play_width), static_cast<float>(screen_h) };
        SDL_RenderFillRect(renderer, &rightBar);

        // Draw game over or victory message
        if (game_over) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 200);
            SDL_FRect overlay = { static_cast<float>(play_x), static_cast<float>(screen_h / 2 - 50),
                                 static_cast<float>(play_width), 100.0f };
            SDL_RenderFillRect(renderer, &overlay);
        }
        else if (victory) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 200);
            SDL_FRect overlay = { static_cast<float>(play_x), static_cast<float>(screen_h / 2 - 50),
                                 static_cast<float>(play_width), 100.0f };
            SDL_RenderFillRect(renderer, &overlay);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    // Cleanup
    if (bg_texture) SDL_DestroyTexture(bg_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}