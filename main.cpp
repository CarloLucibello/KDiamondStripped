#include <iostream>
#include <fstream>
#include "benchmarksuite.h"
#include "game.h"
#include "player.h"
#include "options.h"

using namespace std;

int main(int argc, char *argv[]){
    //Valori di defailt degli argomenti opzionali
    int seedPlayer = -1;
    int seedGame = -1;
	string work = "play";     // "play", "test"
	bool verbose = true;
	double qi = 1;
	string outPath = "res.txt";
    int level = 1;
    int niter = 1000;  //da usare nei test
    options::Opt opt = {seedPlayer, seedGame, work, verbose, qi, outPath, level, niter};
	opt.parseOptions(argc, argv);

    Game * game = new Game(seedGame, verbose);


    BenchmarkSuite bench(game);
    if(work == "play"){
        //questo è per fare una singola partita col giocatore intelligente
        bench.singleGame(level, qi, seedPlayer, verbose);
        if(game->isWon()){
            cout << "Partita Vinta!!!! " << endl;
        } else {
            cout << "Partita Persa :( " << endl;
        }

        //questo è per fare una singola mossa col giocatore intelligente
//        bench.singleMove(level, qi, seedPlayer, verbose);
    }

     if(work == "test"){
         string paramsPath = "exp1.txt";
         bench.fullTestLevel( qi, niter, seedPlayer, verbose, paramsPath, outPath);
    }

    return 0;
}
