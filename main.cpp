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
	options::Opt opt = {seedPlayer, seedGame, work, verbose, qi, outPath};
	opt.parseOptions(argc, argv);

    Game * game = new Game(seedGame, verbose);


    BenchmarkSuite bench(game);
    if(work == "play"){
        //questo è per fare una singola partita col giocatore intelligente
        bench.singleGame(1, qi, seedPlayer, verbose);
    }

     if(work == "test"){
        ofstream myfile(outPath);

        auto res = bench.testLevel(1, 1000, seedPlayer, true);

        cout <<"######### RESULTS ############" << endl;
        cout << "ProbWin  " << res.probWin.mean() << endl;
        cout << "AveMoves  " << res.aveMoves.mean() << " " << res.aveMoves.stdDev() << endl;
        cout << "AvePoints  " << res.avePoints.mean() << " " << res.avePoints.stdDev() << endl;
        res.print(myfile);
        myfile.close();
    }


    return 0;
}
