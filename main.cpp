#include <iostream>
#include <fstream>
#include <sstream> 
#include "benchmarksuite.h"
#include "game.h"
#include "player.h"
#include "options.h"

// i parametri del livello vengono stabiliti in levels/params.txt
// ogni linea di questo file corrisponde a un livello
// per il momento ci sono 19 livelli, tutti uguali, a parte la maschera che cambia.
// potete cambiare i parametri di un certo livello agendo direttamente
// su questo file
// il livello a cui giocate, di default è fissato ad 1 nel main.
// per cambiarlo, e giocare per esempio al livello 3, basta lanciare il
// programma con l'opzione -l 3. Vedi options.h

using namespace std;

int main(int argc, char *argv[]){
    //Valori di defailt degli argomenti opzionali
    int seedPlayer = -1;
    // questo è il seme con cui viene inizializzata la creazione della matrice
    int seedGame = -1;
    // questo è il seme con cui vengono inizializzate le mosse del giocatore
    // questi valori possono essere cambiati lanciando il programma con le
    // opzioni -p e -g. Vedi options.h
    
	string work = "play";     // "play", "test"
	bool verbose = true;
    // verbose = true significa che ci facciamo stampare ogni mossa che facciamo
    // e riaggiorniamo continuamente la matrice con il i dimanti che cancelliamo
    // e quelli che vengono ricreati.
	double qi = 1;
    // qi è l'intelligenza del giocatore: 1 significa che è molto intelligente,
    // 0 significa che fa una mossa a caso.
	string outPath = "res.txt";
    int level = 1;
    int niter = 1000;
    // niter è il numero di partite che facciamo fare al giocatore
    // la probabilità di passare il livello la misuriamo come
    // numero di partite vinte / niter
    
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

    }

     if(work == "test"){
         bench.fullTestParam(qi, niter, seedPlayer, verbose);
       // auto res = bench.testLevel(level, qi, niter, seedPlayer, verbose);
       // res.print(cout);
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
