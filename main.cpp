#include <iostream>
using namespace std;

#include "player.h"
#include "game.h"


int main(){

    Game* game = new Game;
    game->setMode(Mode::UntimedGame);
    game->setLevel(0);  //from 0 to 4
    game->startNewGame();
//
    Player* player = new Player(game);

    cout << "***BOARD*****" << endl;
    game->printBoard();
    cout << endl;
    game->getMoves();
    cout << "***MOVES*****" << endl;
    game->printMoves();
    cout << endl;

    player->playRandomMove();

    cout << "***BOARD*****" << endl;
    game->printBoard();
    cout << endl;

    game->getMoves();
    cout << "***MOVES*****" << endl;
    game->printMoves();
    cout << endl;




    delete game;

    return 0;
}
