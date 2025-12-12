#include "gamestate.h"

bool GameState::isPaused() const{
    return paused;
}

bool GameState::isGameOver() const{
    return game_over;
}

bool GameState::isVictory() const{
    return victory;
}

void GameState::setPaused(bool p){
    paused = p;
}

void GameState::setGameOver(bool go){
    game_over = go;
}

void GameState::setVictory(bool v){
    victory = v;
}


void GameState::togglePause(){
    paused = !paused;
}

void GameState::reset() {
    paused = false;
    game_over = false;
    victory = false;
}

bool GameState::isActive(){
    return !paused && !game_over && !victory;
}