#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>
#include "entity.h"

int main(int argc, char* argv[]) {
    // SDL3 initialization
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error SDL_Init: %s", SDL_GetError());
        return 1;
    }

    // Window creation
    SDL_Window* window = SDL_CreateWindow(
        "Controls",
        800, 600,
        SDL_WINDOW_RESIZABLE
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

    // Entity creation
    Entity player;
    Entity_Init(&player, 400.0f, 300.0f, 50.0f, 50.0f, 200.0f, renderer);

    // Main loop
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

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        Entity_Draw(&player, renderer);

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
