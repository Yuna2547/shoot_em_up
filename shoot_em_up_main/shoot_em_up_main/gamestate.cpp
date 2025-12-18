#include "gamestate.h"

//each states used for different menu set ups
bool GameState::isPaused() const{
    return paused;
}

bool GameState::isGameOver() const{
    return game_over;
}

bool GameState::isVictory() const{
    return victory;
}

void GameState::setPaused(bool pausedGame){
    paused = pausedGame;
}

void GameState::setGameOver(bool gameOverGame){
    game_over = gameOverGame;
}

void GameState::setVictory(bool victoryGame){
    victory = victoryGame;
}


void GameState::togglePause(){
    paused = !paused;
}

void GameState::reset() {
    paused = false;
    game_over = false;
    victory = false;
}

bool GameState::isActive() const{
    return !paused && !game_over && !victory;
}