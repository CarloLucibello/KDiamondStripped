#include <iostream>
#include "benchmarksuite.h"
#include "game.h"

using namespace std;

int main(int argc, char *argv[]){
    
    if (argc!=4){
        fprintf(stderr, "\nSyntax:\nInserisci il seed_player il seed_board e il qi\n");
        cout << "il qi e' un numero tra 0 e 1 che indica l'intelligenza del giocatore; " << endl;
        cout << "se qi=1, il giocatore sceglie sempre la migliore mossa possibile. " << endl;
        
        return 1;
    }
    
    LinCongRNG::UInt8 seed_player=atoll(argv[1]);
    LinCongRNG::UInt8 seed_board=atoll(argv[2]);
    
    //qi è un numero compreso tra 0 e 1 che indica l'intelligenza del giocatore
    //qi=1 -> giocatore sceglie sempre la mossa migliore da fare
    //qi=0 -> giocatore sceglie sempre una mossa random
    //in questo caso, i metodi playRandomMove() e playRandomMove_smarter() sono
    //equivalenti
    
    double qi=atof(argv[3]);

    Game * game = new Game();
    game->setMode(Mode::UntimedGame);

    BenchmarkSuite bench(game);
    
    //questo è per fare una singola partita col giocatore intelligente
    
    int numero_di_mosse=3;
    bench.Singlegame(game, qi, numero_di_mosse, seed_player, seed_board);
    
    /*
    BenchmarkSuite bench(game);

    auto res = bench.testLevel(0);

    cout <<"######### RESULTS ############" << endl;
    cout << "ProbWin  " << res.probWin.mean() << endl;
    cout << "AveMoves  " << res.aveMoves.mean() << " " << res.aveMoves.stdDev() << endl;
    cout << "AvePoints  " << res.avePoints.mean() << " " << res.avePoints.stdDev() << endl;
     
    */

    return 0;
}
