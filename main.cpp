#include <iostream>
using namespace std;

#include "game.h"


int main(){

    Game* game = new Game;
    game->setMode(Mode::UntimedGame);
    game->setLevel(0);  //from 0 to 4
    game->startNewGame();
    game->printBoard();
    delete game;

    return 0;
}
