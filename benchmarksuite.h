#ifndef BENCHMARKSUITE_H
#define BENCHMARKSUITE_H

#include "game.h"
#include "player.h"
#include "observable.h"

class BenchmarkResults{
public:
    int level;
    int niter;

    //OSSERVABILI
    Observable probWin;  //probabilità di superare il livello
    Observable aveMoves;  //numero medio di passi che ci voglio a raggiungere l'obiettivo punteggio
    Observable avePoints;  //puteggio medio ottenuto nel numero di mosse concesso


    void print(ostream& fout);

};

class BenchmarkSuite{
public:
    BenchmarkSuite(Game* game);
    BenchmarkResults testLevel(int level, double qi, int niter = 1000, int seed = -1, bool verbose = false);
    void singleGame(int level, double qi, int seedPlayer = -1, bool verbose = false);  //verbose si potrebbe eliminare da qua
    void singleMove(int level, double qi, int seedPlayer = -1, bool verbose = false);  //verbose si potrebbe eliminare da qua



private:
    Game * m_game;
};

#endif // BENCHMARKSUITE_H
