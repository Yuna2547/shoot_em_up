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
		0,0,
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
	int screen_w =0, screen_h =0;
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
	} else {
		SDL_Log("Warning: could not load background image: %s", SDL_GetError());
	}

	// Entity creation (centered in screen) using C++ class
	Entity player(screen_w /2.0f, screen_h /2.0f,
		100.0f,100.0f, // sprite draw size
		200.0f, // movement speed
		renderer);
	player.SetScreenBounds(screen_w, screen_h);

	// bullet manager creation (0.5 second cooldown)
	BulletManager bulletManager;
	BulletManager_Init(&bulletManager,0.1f);

	// main loop
	bool running = true;
	SDL_Event event;
	Uint64 last_time = SDL_GetTicks();

	while (running) {
		// Delta time
		Uint64 current_time = SDL_GetTicks();
		float dt = (current_time - last_time) /1000.0f;
		last_time = current_time;

		// Event handling
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}

		// Update entity
		const bool* keys = SDL_GetKeyboardState(NULL);
		player.Update(keys, dt);

		// Update bullet manager cooldown
		BulletManager_Update(&bulletManager, dt);

		// Shoot when space is pressed
		if (keys[SDL_SCANCODE_SPACE]) {
			// Calculate bullet starting position (center top of player)
			float bullet_x = player.rect.x + (player.rect.w /2.0f) -2.5f;
			float bullet_y = player.rect.y;
			BulletManager_Shoot(&bulletManager, bullet_x, bullet_y);
		}

		if (keys[SDL_SCANCODE_ESCAPE]) {
			running = false;
		}

		// Update all bullets
		BulletManager_UpdateBullets(&bulletManager, dt,600);

		// Clear screen
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);

		// Draw background
		if (bg_texture) {
			SDL_RenderTexture(renderer, bg_texture, NULL, NULL);
		}

		// Draw player
		player.Draw(renderer);

		// Draw bullets
		BulletManager_Draw(&bulletManager, renderer);

		SDL_RenderPresent(renderer);

		//60 fps limit
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
