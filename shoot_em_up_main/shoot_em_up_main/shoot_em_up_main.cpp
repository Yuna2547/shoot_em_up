#include <SDL3/SDL.h>
#include "entity.h"
#include "bullet.h"

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error SDL_Init: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Controls", 800, 600, SDL_WINDOW_RESIZABLE);
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

    // Player entity (now a class)
    Entity player(400.0f, 300.0f, 50.0f, 50.0f, 200.0f);

    // Bullet manager (already a class)
    BulletManager bulletManager(100, 0.1f);

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

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

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
