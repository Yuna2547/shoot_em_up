#include "game.h"

#include <format>
#include <print>

Game::Game() : window(nullptr), renderer(nullptr), bgTexture(nullptr), screenWidth(0), screenHeight(0), playAreaX(0),
    playAreaWidth(0), player(nullptr), bulletManager(nullptr), enemyBulletManager(nullptr), enemyManager(nullptr), gameState(nullptr),
    gameMenu(nullptr), initialPlayerX(0.0f), initialPlayerY(0.0f), running(true), lastTime(0) {
    
}

Game::~Game() {
    cleanup();
}

bool Game::initialize() {
    if (!initSDL() || !createWindow() || !createRenderer()) 
        return false;
    calculatePlayArea();
    if (!loadResources()) 
        return false;
    setupGameObjects();
    return true;
}

bool Game::initSDL() const {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error SDL_Init: ", SDL_GetError());
        return false;
    }
    return true;
}

bool Game::createWindow() {
    window = SDL_CreateWindow("Shoot 'Em Up", 800, 600, SDL_WINDOW_FULLSCREEN);
    if (!window) {
        SDL_Log("Error creating window: ", SDL_GetError());
        SDL_Quit();
        return false;
    }
    SDL_GetWindowSize(window, &screenWidth, &screenHeight);
    return true;
}

bool Game::createRenderer(){
    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        SDL_Log("Error creating renderer: ", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    return true;
}

void Game::calculatePlayArea() {
    playAreaWidth = static_cast<int>(screenWidth * 0.35f);
    playAreaX = (screenWidth - playAreaWidth) / 2;
    SDL_Log("Screen: ", screenWidth, " x ", screenHeight, ", Play area: x = ", playAreaX, "width = ", playAreaWidth);
}

bool Game::loadResources()
{
    SDL_Surface* bgSurface = IMG_Load("assets/background.png");  
    if (bgSurface) {
        bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_DestroySurface(bgSurface);
        if (!bgTexture) 
            SDL_Log("Warning: could not create background texture: ", SDL_GetError());
    }
    else 
        SDL_Log("Warning: could not load background image: ", SDL_GetError());
    return true;
}

void Game::setupGameObjects() {
    initialPlayerX = playAreaX + playAreaWidth / 2.0f - 50.0f;
    initialPlayerY = screenHeight - 150.0f;

    player = new Entity(initialPlayerX, initialPlayerY, 100.0f, 130.0f, 200.0f, renderer);
    bulletManager = new BulletManager(100, 0.1f);
    enemyBulletManager = new EnemyBulletManager(200, 0.5f);
    enemyManager = new EnemyManager();
    gameState = new GameState();
    gameMenu = new Menu(renderer, screenWidth, screenHeight);

    
    player->setScreenBounds(playAreaWidth, screenHeight);
    player->setOffsetX(playAreaX);
    enemyManager->setupEnemies(renderer, playAreaX, playAreaWidth, screenHeight);
    enemyManager->setBulletManager(enemyBulletManager);
    
}

bool Game::showMenu() {
    bool inMenu = true;
    bool startGame = false;

    while (inMenu) {
        SDL_Event menuEvent;
        float mouseX;
        float mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        gameMenu->update(mouseX, mouseY);
        while (SDL_PollEvent(&menuEvent)) {
            if (menuEvent.type == SDL_EVENT_QUIT) 
                inMenu = false;
            if (menuEvent.type == SDL_EVENT_WINDOW_RESIZED) {
                SDL_GetWindowSize(window, &screenWidth, &screenHeight);
                gameMenu->setWindowSize(screenWidth, screenHeight);
            }
            int menuResult = gameMenu->handleEvents(menuEvent);
            if (menuResult == 1) {
                startGame = true;
                inMenu = false;
            }
            else if (menuResult == 2) 
                inMenu = false;
        }
        gameMenu->draw();

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }
    return startGame;
}

void Game::run() {
    lastTime = SDL_GetTicks();
    while (running) {
        Uint64 currentTime = SDL_GetTicks();
        float dt = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        handleEvents();
        update(dt);
        render();

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

void Game::handleEvents(){
    SDL_Event event;
    float mouseX;
    float mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // ---- Event handling (only handle events here) ----
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) 
            running = false;

        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == SDLK_R) 
                resetGame();

            if (event.key.key == SDLK_ESCAPE) {
                if (!gameState->isGameOver() && !gameState->isVictory() && !gameState->isGameOver()) {
                    gameState->togglePause();
                    gameMenu->setPauseMode(gameState->isPaused());
                    gameMenu->setVictoryMode(false);
                }
            }
        }

        // If paused, give the menu a chance to handle events
        if (gameState->isPaused() || gameState->isVictory() || gameState->isGameOver()) {
            int menuResult = gameMenu->handleEvents(event);
            if (menuResult == 1) {
                if (gameState->isVictory() || gameState->isGameOver()) 
                    resetGame();
                else 
                    gameState->setPaused(false);
            }
            else if (menuResult == 2) 
                running = false;
        }
    }
}

void Game::update(float dt) {
    if (gameState->isActive()) {
        const bool* keys = SDL_GetKeyboardState(nullptr);
        player->update(keys, dt);

        if (keys[SDL_SCANCODE_SPACE]) {
            float bullet_x = player->getRect().x + (player->getRect().w / 2.0f) - 2.5f;
            float bullet_y = player->getRect().y;
            bulletManager->shoot(bullet_x, bullet_y);
        }

        bulletManager->update(dt);
        bulletManager->updateBullets(dt);
        enemyManager->update(dt);
        enemyBulletManager->update(dt);
        enemyBulletManager->updateBullets(dt, screenHeight);

        handleCollisions();

        if (player->getHealth() <= 0) 
            handleGameOver();

        if (enemyManager->allDestroyed()) 
            handleVictory();
    }
    else if (gameState->isPaused())
    {
        float mx, my;
        SDL_GetMouseState(&mx, &my);
        gameMenu->update(mx, my);
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (bgTexture) 
        SDL_RenderTexture(renderer, bgTexture, nullptr, nullptr);

    enemyManager->draw();
    player->draw(renderer);
    bulletManager->draw(renderer);
    enemyBulletManager->draw(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_FRect leftBar = { 0.0f, 0.0f, static_cast<float>(playAreaX), static_cast<float>(screenHeight) };
    SDL_RenderFillRect(renderer, &leftBar);

    SDL_FRect rightBar = { static_cast<float>(playAreaX + playAreaWidth), 0.0f, static_cast<float>(screenWidth - playAreaX - playAreaWidth), static_cast<float>(screenHeight) };
    SDL_RenderFillRect(renderer, &rightBar);

    if (gameState->isPaused() || gameState->isVictory() || gameState->isGameOver()) 
        gameMenu->draw();
}

void Game::handleCollisions() {
    checkOffscreenEnemies();
    checkBulletEnemyCollisions();
    checkPlayerEnemyCollisions();
    checkPlayerBulletCollisions();
}

void Game::checkBulletEnemyCollisions() {
    for (auto& bullet : bulletManager->getBullets()) {
        if (!bullet.active)
            continue;
        for (auto& enemy : enemyManager->getEnemies()) {
            if (!enemy.isAlive()) 
                continue;
            if (checkCollision(bullet.getRect(), enemy.getRect())) {
                enemy.takeDamage(2);
                bullet.deactivate();
                break;
            }
        }
    }
}

void Game::checkPlayerEnemyCollisions(){
    const SDL_FRect& playerRect = player->getRect();
    for (auto& enemy : enemyManager->getEnemies()) {
        if (!enemy.hasCollided() && enemy.isAlive() && checkCollision(playerRect, enemy.getRect())) {
            player->takeDamage(3);
            enemy.setCollided();
        }
    }
    if (enemyBulletManager->canShoot()) 
        enemyManager->shootFromRandomEnemy();
}

void Game::checkPlayerBulletCollisions(){
    const SDL_FRect& playerRect = player->getRect();

    for (auto& enemyBullet : enemyBulletManager->getBullets()) {
        if (!enemyBullet.active) 
            continue;

        if (checkCollision(playerRect, enemyBullet.getRect())) {
            player->takeDamage(2);
            enemyBullet.deactivate();
        }
    }
}

void Game::checkOffscreenEnemies() {
    for (auto& enemy : enemyManager->getEnemies()) {
        if (enemy.isAlive() && !enemy.hasCollided() && enemy.isOffScreen(screenHeight)) {
            player->takeDamage(1);
            enemy.setCollided();
        }
    }
}

bool Game::checkCollision(const SDL_FRect& a, const SDL_FRect& b) const {
    return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
}

void Game::resetGame() {
    player->resetPosition(initialPlayerX, initialPlayerY);
    player->resetHealth();

    delete enemyBulletManager;
    delete bulletManager;
    bulletManager = new BulletManager(100, 0.1f);
    enemyBulletManager = new EnemyBulletManager(200, 0.5f); 

    enemyManager->setupEnemies(renderer, playAreaX, playAreaWidth, screenHeight);
    enemyManager->setBulletManager(enemyBulletManager);

    gameState->reset();
}

void Game::handleGameOver(){
    gameState->setGameOver(true);
    gameState->setPaused(true);
    gameMenu->setPauseMode(true);
    gameMenu->setGameOverMode(true);
    
}

void Game::handleVictory(){
    gameState->setVictory(true);
    gameState->setPaused(true);
    gameMenu->setPauseMode(true);
    gameMenu->setVictoryMode(true);
}

void Game::handlePause() const{
    gameState->setPaused(true);
    gameMenu->setPauseMode(true);
}



void Game::cleanup() {
    delete player;
    delete bulletManager;
    delete enemyManager;
    delete gameState;
    delete gameMenu;

    if (bgTexture) 
        SDL_DestroyTexture(bgTexture);
    if (renderer) 
        SDL_DestroyRenderer(renderer);
    if (window) 
        SDL_DestroyWindow(window);

    SDL_Quit();
}
