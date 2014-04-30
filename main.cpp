#include <iostream>
#include "benchmarksuite.h"
#include "game.h"
#include "player.h"

using namespace std;

int main(int argc, char *argv[]){

    if (argc!=4){
        fprintf(stderr, "\nSyntax:\nInserisci il seedPlayer il seedGame e il qi\n");
        cout << "il qi e' un numero tra 0 e 1 che indica l'intelligenza del giocatore; " << endl;
        cout << "se qi=1, il giocatore sceglie sempre la migliore mossa possibile. " << endl;

        return 1;
    }

    int seedPlayer = atoll(argv[1]);
    int seedGame = atoll(argv[2]);

    //qi è un numero compreso tra 0 e 1 che indica l'intelligenza del giocatore
    //qi=1 -> giocatore sceglie sempre la mossa migliore da fare
    //qi=0 -> giocatore sceglie sempre una mossa random
    //in questo caso, i metodi playRandomMove() e playSmartRandomMove() sono
    //equivalenti

    double qi = atof(argv[3]);

    Game * game = new Game(seedGame);
    BenchmarkSuite bench(game);

    //questo è per fare una singola partita col giocatore intelligente

    bench.singleGame(qi, seedPlayer);


//
//    BenchmarkSuite bench(game);
//
//    auto res = bench.testLevel(0);
//
//    cout <<"######### RESULTS ############" << endl;
//    cout << "ProbWin  " << res.probWin.mean() << endl;
//    cout << "AveMoves  " << res.aveMoves.mean() << " " << res.aveMoves.stdDev() << endl;
//    cout << "AvePoints  " << res.avePoints.mean() << " " << res.avePoints.stdDev() << endl;


    return 0;
}
