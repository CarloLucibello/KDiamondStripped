#include <iostream>
using namespace std;

#include "player.h"
#include "game.h"


int main(int argc, char *argv[]){
    
    if (argc!=3){
        fprintf(stderr, "\nSyntax:\nInserisci il seed_player e il seed_board \n\n");
        return 1;
    }
    
    UInt8 seed_player=atoll(argv[1]);
    UInt8 seed_board=atoll(argv[2]);

    
    Game* game = new Game;
    game->setMode(Mode::UntimedGame);
    game->setLevel(0);  //from 0 to 4
    game->startNewGame(seed_board);
    
    
    Player* player = new Player(game,seed_player);
    
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
