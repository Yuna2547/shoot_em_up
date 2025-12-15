#include "Game.h"

int main(int argc, char* argv[]) {
    Game game;

    if (!game.initialize()) 
        return 1;

    if (!game.showMenu()) 
        return 0;

    game.run();

    return 0;
}