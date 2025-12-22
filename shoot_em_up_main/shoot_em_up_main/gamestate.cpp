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

int GameState::getScore() const{
    return score;
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

void GameState::addScore(int points){
    score += points;
}


void GameState::togglePause(){
    paused = !paused;
}

void GameState::reset() {
    paused = false;
    game_over = false;
    victory = false;
    score = 0;
}

bool GameState::isActive() const{
    return !paused && !game_over && !victory;
}