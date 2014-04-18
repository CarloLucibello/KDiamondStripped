#ifndef BENCHMARKSUITE_H
#define BENCHMARKSUITE_H

#include "game.h"
#include "player.h"

class BenchmarkResults{
public:
    int level;
    int niter;

    //OSSERVABILI
    double probWin;  //probabilità di superare il livello
    double aveMoves;  //numero medio di passi che ci voglio a raggiungere l'obiettivo punteggio
    double avePoints;  //puteggio medio ottenuto nel numero di mosse concesso
};

class BenchmarkSuite{
public:
    BenchmarkSuite(Game* game);
    BenchmarkResults testLevel(int level, int niter = 100);

private:
    Game* m_game;
};

#endif // BENCHMARKSUITE_H
