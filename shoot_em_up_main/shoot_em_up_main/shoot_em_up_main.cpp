#include <SDL3/SDL.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    // Initialisation de SDL3
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Erreur SDL_Init: %s", SDL_GetError());
        return 1;
    }

    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow(
        "Déplacement SDL3",
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

    // Position et taille de l'élément
    SDL_FRect rect = { 400.0f, 300.0f, 50.0f, 50.0f };

    // Vitesse de déplacement
    float speed = 200.0f; // pixels par seconde

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

        // Gestion du clavier (état continu)
        const bool* keys = SDL_GetKeyboardState(NULL);
        
        if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) {
            rect.y -= speed * dt;
        }
        if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) {
            rect.y += speed * dt;
        }
        if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
            rect.x -= speed * dt;
        }
        if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
            rect.x += speed * dt;
        }

        // Limiter l'élément à l'écran
        if (rect.x < 0) rect.x = 0;
        if (rect.y < 0) rect.y = 0;
        if (rect.x + rect.w > 800) rect.x = 800 - rect.w;
        if (rect.y + rect.h > 600) rect.y = 600 - rect.h;

        // Rendu
        // Effacer l'écran (noir)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Dessiner le rectangle (vert)
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);

        // Afficher
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