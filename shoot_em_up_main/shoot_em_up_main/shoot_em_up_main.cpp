#include "game.h"

int main(int argc, char* argv[]) {
    Game game;
    //short main only to check if the game is initialized and if the menu shows up
    if (!game.initialize()) 
        return 1;
    if (!game.showMenu()) 
        return 0;
    game.run();
    return 0;
}