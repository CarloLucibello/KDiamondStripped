#include <iostream>
using namespace std;

#include "benchmarksuite.h"
#include "game.h"



int main(int argc, char *argv[]){

    if (argc!=3){
        fprintf(stderr, "\nSyntax:\nInserisci il seed_player e il seed_board \n\n");
        return 1;
    }

    Game * game = new Game();
    game->setMode(Mode::UntimedGame);

    BenchmarkSuite bench(game);

    auto res = bench.testLevel(0, 10);
    cout << res.probWin << endl;
    cout << res.aveMoves << endl;
    cout << res.avePoints << endl;


    return 0;
}
