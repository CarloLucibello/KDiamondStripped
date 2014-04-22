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

    auto res = bench.testLevel(0);

    cout <<"######### RESULTS ############" << endl;
    cout << "ProbWin  " << res.probWin.mean() << endl;
    cout << "AveMoves  " << res.aveMoves.mean() << " " << res.aveMoves.stdDev() << endl;
    cout << "AvePoints  " << res.avePoints.mean() << " " << res.avePoints.stdDev() << endl;

    return 0;
}
