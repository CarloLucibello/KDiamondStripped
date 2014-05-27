#include <iostream>
#include <fstream>
#include <sstream> 
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
         bench.fullTestParam(qi, niter, seedPlayer, verbose);
       // auto res = bench.testLevel(level, qi, niter, seedPlayer, verbose);
      //  res.print(cout);
    }
    if(work == "mask"){
        string paramsPath = "Params.txt";
        string resPath;
        
        paramsPath= "mask" + paramsPath;
        resPath= "mask" + outPath;
        
        bench.testParam( "mask", "levels/params.txt", paramsPath, 1);
        bench.fullTestLevel( qi, niter, seedPlayer, verbose, paramsPath, resPath);
    }
    return 0;
}
