#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>
#include "entity.h"
#include "bullet.h"

int main(int argc, char* argv[]) {
    // SDL3 initialization
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error SDL_Init: %s", SDL_GetError());
        return 1;
    }

    // Window creation
    SDL_Window* window = SDL_CreateWindow(
        "Controls",
        0, 0,
        SDL_WINDOW_FULLSCREEN
    );
    if (!window) {
        SDL_Log("Error during the window's creation: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Renderer creation
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Error renderer's creation: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Query actual screen size
        int screen_w = 0, screen_h = 0;
    SDL_GetWindowSize(window, &screen_w, &screen_h);

    // Entity creation (centered in screen)
    Entity player;
    Entity_Init(&player,
        screen_w / 2.0f, screen_h / 2.0f,
        100.0f, 100.0f,   // sprite draw size
        200.0f,           // movement speed
        renderer);
    Entity_SetScreenBounds(&player, screen_w, screen_h);
    // bullet manager creation (0.5 second cooldown)
    BulletManager bulletManager;
    BulletManager_Init(&bulletManager, 0.5f);

    // main loop
    bool running = true;
    SDL_Event event;
    Uint64 last_time = SDL_GetTicks();

    while (running) {
        // Delta time
        Uint64 current_time = SDL_GetTicks();
        float dt = (current_time - last_time) / 1000.0f;
        last_time = current_time;

        // Event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        // Update entity
        const bool* keys = SDL_GetKeyboardState(NULL);
        Entity_Update(&player, keys, dt);

        // Shoot when space is pressed
        if (keys[SDL_SCANCODE_SPACE]) {
            // Calculate bullet starting position (center top of player)
            float bullet_x = player.rect.x + (player.rect.w / 2.0f) - 2.5f;
            float bullet_y = player.rect.y;
            BulletManager_Shoot(&bulletManager, bullet_x, bullet_y);
        }

        // Inside your main loop, each frame:
        const bool* key = SDL_GetKeyboardState(NULL);
        if (key[SDL_SCANCODE_ESCAPE]) {
            running = false;
        }


        // Update all bullets
        BulletManager_UpdateBullets(&bulletManager, dt, 600);

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw player
        Entity_Draw(&player, renderer);

        // Draw bullets
        BulletManager_Draw(&bulletManager, renderer);

        SDL_RenderPresent(renderer);

        // 60 fps limit
        SDL_Delay(16);
    }

    // Cleanup
    Entity_Destroy(&player);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
