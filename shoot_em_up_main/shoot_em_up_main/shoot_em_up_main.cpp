#include <SDL3/SDL.h>
#include <stdbool.h>
#include "entity.h"

int main(int argc, char* argv[]) {
    // Initialisation de SDL3
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Erreur SDL_Init: %s", SDL_GetError());
        return 1;
    }

    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow(
        "Déplacement SDL3 - Modulaire",
        800, 600,
        SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        SDL_Log("Erreur création fenêtre: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Création du renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Erreur création renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Création de l'entité
    Entity player;
    Entity_Init(&player, 400.0f, 300.0f, 50.0f, 50.0f, 200.0f);

    // Boucle principale
    bool running = true;
    SDL_Event event;
    Uint64 last_time = SDL_GetTicks();

    while (running) {
        // Calcul du delta time
        Uint64 current_time = SDL_GetTicks();
        float dt = (current_time - last_time) / 1000.0f;
        last_time = current_time;

        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        // Mise à jour de l'entité
        const bool* keys = SDL_GetKeyboardState(NULL);
        Entity_Update(&player, keys, dt);

        // Rendu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        Entity_Draw(&player, renderer);

        SDL_RenderPresent(renderer);

        // Limiter à ~60 FPS
        SDL_Delay(16);
    }

    // Nettoyage
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}