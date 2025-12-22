#pragma once

#include <SDL3/SDL.h>
#include "entity.h"
#include "bullet.h"
#include "enemy.h"
#include "menu.h"
#include "gamestate.h"
#include <SDL3_image/SDL_image.h>
#include <string>

class Game {
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
	EnemyBulletManager* enemyBulletManager;
	EnemyManager* enemyManager;
	GameState* gameState;
	Menu* gameMenu;
	float initialPlayerX;
	float initialPlayerY;

	bool running;
	Uint64 lastTime;

	// level support
	int currentLevel;
	std::string getLevelFilename(int level) const;
	void loadLevel(int level);

	bool initSDL() const;
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
	void checkPlayerBulletCollisions();
	void checkOffscreenEnemies();
	bool checkCollision(const SDL_FRect& a, const SDL_FRect& b) const;

	void resetGame();
	void handleGameOver();
	void handleVictory();
	void handlePause() const;
	void drawScore() const;

	void cleanup();

public:
	Game();
	~Game();

	bool initialize();
	bool showMenu();
	void run();

};