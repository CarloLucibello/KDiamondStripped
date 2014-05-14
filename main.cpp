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
    options::Opt opt = {seedPlayer, seedGame, work, verbose, qi, outPath, level};
	opt.parseOptions(argc, argv);

    Game * game = new Game(seedGame, verbose);


    BenchmarkSuite bench(game);
    if(work == "play"){
        //questo è per fare una singola partita col giocatore intelligente
        bench.singleGame(level, qi, seedPlayer, verbose);

        //questo è per fare una singola mossa col giocatore intelligente
//        bench.singleMove(level, qi, seedPlayer, verbose);
    }

     if(work == "test"){
        ofstream myfile(outPath);
        int niter = 1000;

        auto res = bench.testLevel(level, qi, niter, seedPlayer, true);

        cout <<"######### RESULTS ############" << endl;
        cout << "ProbWin  " << res.probWin.mean() << endl;
        cout << "AveMoves  " << res.aveMoves.mean() << " " << res.aveMoves.stdDev() << endl;
        cout << "AvePoints  " << res.avePoints.mean() << " " << res.avePoints.stdDev() << endl;
        res.print(myfile);
        myfile.close();
    }

    return 0;
}
