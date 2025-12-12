#pragma once

#include <SDL3/SDL.h>
#include "entity.h"
#include "bullet.h"
#include "enemy.h"
#include "menu.h"
#include "gamestate.h"
#include <SDL3_image/SDL_image.h>

class Game{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* bgTexture;
	int screenWidth;
	int screenHeight;
	int playAreaX;
	int playAreaWidth;

	Entity* player;
	BulletManager* bulletManager;
	EnemyManager* enemyManager;
	GameState* gameState;
	Menu* gameMenu;
	float initialPlayerX;
	float initialPlayerY;
	
	bool running;
	Uint64 lastTime;

	bool initSDL();
	bool createWindow();
	bool createRenderer();
	void calculatePlayArea();
	bool loadResources();
	void setupGameObjects();
	void handleEvents();
	void update(float dt);
	void render();
	void handleCollisions();
	void checkBulletEnemyCollisions();
	void checkPlayerEnemyCollisions();
	void checkOffscreenEnemies();
	bool checkCollision(const SDL_FRect& a, const SDL_FRect& b);
	void resetGame();
	void handleGameOver();
	void handleVictory();
	void handlePause();
	void updateMenu(float mouseX, float mouseY);
	int processMenuResult(int menuResult);

	void cleanup();

public:
	Game();
	~Game();

	bool initialize();
	bool showMenu();
	void run();

};

