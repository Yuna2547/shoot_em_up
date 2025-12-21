#include "game.h"

#include <format>
#include <print>

Game::Game() : window(nullptr), renderer(nullptr), bgTexture(nullptr), screenWidth(0), screenHeight(0), playAreaX(0), playAreaWidth(0), player(nullptr), bulletManager(nullptr),
enemyBulletManager(nullptr), enemyManager(nullptr), gameState(nullptr), gameMenu(nullptr), initialPlayerX(0.0f), initialPlayerY(0.0f), running(true), lastTime(0), currentLevel(1) {

}

Game::~Game() {
    cleanup();
}

bool Game::initialize() {       //initialisation of the window
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
        printf("Error SDL_Init");
        return false;
    }
    return true;
}

bool Game::createWindow() {         //creation of the window following the parameters
    window = SDL_CreateWindow("Shoot 'Em Up", 800, 600, SDL_WINDOW_FULLSCREEN);
    if (!window) {
        printf("Error creating window");
        SDL_Quit();
        return false;
    }
    SDL_GetWindowSize(window, &screenWidth, &screenHeight);
    return true;
}

bool Game::createRenderer() {        //renderers created
    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        printf("Error creating renderer");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    return true;
}

void Game::calculatePlayArea() {    //calculation of the area where you can play
    playAreaWidth = static_cast<int>(screenWidth * 0.35f);      //define the limits
    playAreaX = (screenWidth - playAreaWidth) / 2;
}

bool Game::loadResources() {        //load the ressources needed for the background
    SDL_Surface* bgSurface = IMG_Load("assets/background.png");
    if (bgSurface) {
        bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_DestroySurface(bgSurface);
        if (!bgTexture)
            printf("Warning: could not create background texture");
    }
    else
        printf("Warning: could not load background image");
    return true;
}

void Game::setupGameObjects() {     //set up of the objects needed throughout the game
    initialPlayerX = playAreaX + playAreaWidth / 2.0f - 50.0f;
    initialPlayerY = screenHeight - 150.0f;

    //set up the parameters and classes associated
    player = new Entity(initialPlayerX, initialPlayerY, 80.0f, 100.0f, 200.0f, renderer);
    bulletManager = new BulletManager(100, 0.1f);
    enemyBulletManager = new EnemyBulletManager(200, 0.5f);
    enemyManager = new EnemyManager();
    gameState = new GameState();
    gameMenu = new Menu(renderer, screenWidth, screenHeight);


    player->setScreenBounds(playAreaWidth, screenHeight);
    player->setOffsetX(playAreaX);
    // load current level file (currentLevel defaults to 1)
    loadLevel(currentLevel);
    enemyManager->setBulletManager(enemyBulletManager);

}

bool Game::showMenu() {     //creation of the menu when the game is started (start menu)
    bool inMenu = true;
    bool startGame = false;

    while (inMenu) {
        SDL_Event menuEvent;
        float mouseX;
        float mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        gameMenu->update(mouseX, mouseY);
        while (SDL_PollEvent(&menuEvent)) {
            if (menuEvent.type == SDL_EVENT_QUIT)      //quit the program
                inMenu = false;
            if (menuEvent.type == SDL_EVENT_WINDOW_RESIZED) {       //can resize
                SDL_GetWindowSize(window, &screenWidth, &screenHeight);
                gameMenu->setWindowSize(screenWidth, screenHeight);
            }

            //menu changes for the game state
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

void Game::run() {      //create dt and get the times needed for any speed in the game
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

void Game::handleEvents() {
    SDL_Event event;
    float mouseX;
    float mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    while (SDL_PollEvent(&event)) {         //events handled here
        if (event.type == SDL_EVENT_QUIT)
            running = false;

        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == SDLK_R)        //used to reset the level
                resetGame();

            if (event.key.key == SDLK_ESCAPE) {     //toggle the menu pause and stop the game from continuing
                if (!gameState->isGameOver() && !gameState->isVictory() && !gameState->isGameOver()) {
                    gameState->togglePause();
                    gameMenu->setPauseMode(gameState->isPaused());
                    gameMenu->setVictoryMode(false);
                }
            }
        }

        if (gameState->isPaused() || gameState->isVictory() || gameState->isGameOver()) {       //anything but menu start
            int menuResult = gameMenu->handleEvents(event);
            
            if (menuResult == 1) {
                if (gameState->isVictory() || gameState->isGameOver()) {
                    resetGame(); 
                }
                else {
                    gameState->setPaused(false); 
                }
            }
            else if (menuResult == 2) {
                if (gameState->isVictory() && currentLevel == 1) {
                    loadLevel(2);
                }
            }
            else if (menuResult == 3) {
                if (gameState->isVictory() && currentLevel == 2) {
                    loadLevel(1);
                }
            }
            else if (menuResult == 4) {
                running = false;
            }
        }
    }
}

void Game::update(float dt) {       //update all objects here
    if (gameState->isActive()) {
        const bool* keys = SDL_GetKeyboardState(nullptr);           //the keyboard entry is always updated
        player->update(keys, dt);

        if (keys[SDL_SCANCODE_SPACE]) {                //the bullets are updated
            float bullet_x = player->getRect().x + (player->getRect().w / 2.0f) - 2.5f;     //bullet adjusted correctly 
            float bullet_y = player->getRect().y;
            bulletManager->shoot(bullet_x, bullet_y);
        }

        //every updates about the bullets and the enemies
        bulletManager->update(dt);
        bulletManager->updateBullets(dt);
        enemyManager->update(dt);
        enemyBulletManager->update(dt);
        enemyBulletManager->updateBullets(dt, screenHeight);

        //update the collisions handled
        handleCollisions();

        if (player->getHealth() <= 0)
            handleGameOver();       //toggle game over if health bellow 0

        if (enemyManager->allDestroyed())
            handleVictory();        //toggle victory if all enemies dead/out of screen and hp remaining for the player
    }
    else if (gameState->isPaused())     //toggle pause menu if paused
    {
        float mx, my;
        SDL_GetMouseState(&mx, &my);
        gameMenu->update(mx, my);
    }
}

void Game::render() {       //renderers for the game
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (bgTexture)
        SDL_RenderTexture(renderer, bgTexture, nullptr, nullptr);

    enemyManager->draw();
    player->draw(renderer);
    bulletManager->draw(renderer);
    enemyBulletManager->draw(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //left bar defined out of play area -> black
    SDL_FRect leftBar = { 0.0f, 0.0f, static_cast<float>(playAreaX), static_cast<float>(screenHeight) };
    SDL_RenderFillRect(renderer, &leftBar);

    //right bar defined out of play area -> black
    SDL_FRect rightBar = { static_cast<float>(playAreaX + playAreaWidth), 0.0f, static_cast<float>(screenWidth - playAreaX - playAreaWidth), static_cast<float>(screenHeight) };
    SDL_RenderFillRect(renderer, &rightBar);

    if (gameState->isPaused() || gameState->isVictory() || gameState->isGameOver())     //render menu in these cases
        gameMenu->draw();
}

void Game::handleCollisions() {     // handle all collisions in the game
    checkOffscreenEnemies();
    checkBulletEnemyCollisions();
    checkPlayerEnemyCollisions();
    checkPlayerBulletCollisions();
}

void Game::checkBulletEnemyCollisions() {       //count the collision between the enemy and the bullet under some conditions
    for (auto& bullet : bulletManager->getBullets()) {
        if (!bullet.active)
            continue;
        for (auto& enemy : enemyManager->getEnemies()) {
            if (!enemy.isAlive())
                continue;
            if (checkCollision(bullet.getRect(), enemy.getRect())) {        //the enemy is damaged 2hp if collision, and the bullet deactivate
                enemy.takeDamage(2);
                bullet.deactivate();
                break;
            }
        }
    }
}

void Game::checkPlayerEnemyCollisions() {        //check if the player collides with the enemy
    const SDL_FRect& playerRect = player->getRect();
    for (auto& enemy : enemyManager->getEnemies()) {
        if (!enemy.hasCollided() && enemy.isAlive() && checkCollision(playerRect, enemy.getRect())) {
            player->takeDamage(3);      //the player loses 3hp if collided with an enemy
            enemy.setCollided();        //cannot collide with this enemy again
        }
    }
    if (enemyBulletManager->canShoot())
        enemyManager->shootFromRandomEnemy();
}

void Game::checkPlayerBulletCollisions() {       //check collision between the enemy's bullet and the player
    const SDL_FRect& playerRect = player->getRect();

    for (auto& enemyBullet : enemyBulletManager->getBullets()) {
        if (!enemyBullet.active)
            continue;

        if (checkCollision(playerRect, enemyBullet.getRect())) {        //the player loses 2hp if the bullet strikes
            player->takeDamage(2);
            enemyBullet.deactivate();
        }
    }
}

void Game::checkOffscreenEnemies() {            //check off screen enemies
    for (auto& enemy : enemyManager->getEnemies()) {
        if (enemy.isAlive() && !enemy.hasCollided() && enemy.isOffScreen(screenHeight)) {   //if an enemy escapes, the player loses 1hp for not killing it
            player->takeDamage(1);
            enemy.setCollided();
        }
    }
}

bool Game::checkCollision(const SDL_FRect& a, const SDL_FRect& b) const {       //system aabb for collisions
    return (a.x < b.x + b.w &&      //A's left edge is to the left of B's right edge
        a.x + a.w > b.x &&          //A's right edge is to the right of B's left edge
        a.y < b.y + b.h &&          //A's top edge is above B's bottom edge
        a.y + a.h > b.y);           //A's bottom edge is below B's top edge
}

void Game::resetGame() {        //takes care of deleting anything not useful for the restart
    player->resetPosition(initialPlayerX, initialPlayerY);      //reset position and health
    player->resetHealth();

    delete enemyBulletManager;          //define everything needed for the restart
    delete bulletManager;
    bulletManager = new BulletManager(100, 0.1f);
    enemyBulletManager = new EnemyBulletManager(200, 0.5f);

    // reload the current level file
    std::string filename = getLevelFilename(currentLevel);
    enemyManager->setupEnemies(renderer, playAreaX, playAreaWidth, screenHeight, filename.c_str());
    enemyManager->setBulletManager(enemyBulletManager);

    gameState->reset();
}

void Game::handleGameOver() {        //sets the game over menu
    gameState->setGameOver(true);
    gameState->setPaused(true);
    gameMenu->setPauseMode(true);
    gameMenu->setGameOverMode(true);

}
void Game::handleVictory() {         //sets the victory menu
    gameState->setVictory(true);
    gameState->setPaused(true);
    gameMenu->setPauseMode(true);
    gameMenu->setVictoryMode(true);

    gameMenu->setCurrentLevel(currentLevel);
}

void Game::handlePause() const {     //set the paused menu
    gameState->setPaused(true);
    gameMenu->setPauseMode(true);
}


void Game::cleanup() {              //cleans every pointers and destroy any texture
    delete player;
    player = nullptr;
    delete bulletManager;
    bulletManager = nullptr;
    delete enemyBulletManager;
    enemyBulletManager = nullptr;
    delete enemyManager;
    enemyManager = nullptr;
    delete gameState;
    gameState = nullptr;
    delete gameMenu;
    gameMenu = nullptr;

    SDL_DestroyTexture(bgTexture);
    bgTexture = nullptr;
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_Quit();
}

std::string Game::getLevelFilename(int level) const {
    if (level == 1) 
        return "setUpEnemy.txt";
    if (level == 2) 
        return "setUpEnemy2.txt";
    return "setUpEnemy.txt";
}

void Game::loadLevel(int level) {
    // clamp level between 1 and 2
    if (level < 1) level = 1;
    if (level > 2) level = 2;
    currentLevel = level;

    // reset player position & health
    if (player) {
        player->resetPosition(initialPlayerX, initialPlayerY);
        player->resetHealth();
    }

    // recreate bullet managers
    delete enemyBulletManager;
    delete bulletManager;
    bulletManager = new BulletManager(100, 0.1f);
    enemyBulletManager = new EnemyBulletManager(200, 0.5f);

    // load enemy file for chosen level
    std::string filename = getLevelFilename(currentLevel);
    enemyManager->setupEnemies(renderer, playAreaX, playAreaWidth, screenHeight, filename.c_str());
    enemyManager->setBulletManager(enemyBulletManager);

    // reset game state (clears victory/pause/game over)
    gameState->reset();
    gameMenu->setPauseMode(false);
    gameMenu->setVictoryMode(false);
    gameMenu->setGameOverMode(false);
}