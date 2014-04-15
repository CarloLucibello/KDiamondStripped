#include <iostream>
using namespace std;

#include "game.h"


int main(){

    Game* game = new Game;
    game->setMode(Mode::UntimedGame);
    game->setLevel(0);  //from 0 to 4
    game->startNewGame();
    game->printBoard();
    cout << endl;
    game->getMoves();
    game->printMoves();
    cout << game->m_availableMoves.size() << endl;
    delete game;

    return 0;
}
