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
};

class BenchmarkSuite{
public:
    void singleGame(Game* game, double qi, int numero_mosse = 5, int seed_player = -1, int seed_board = -1);
    BenchmarkSuite(Game* game);
    BenchmarkResults testLevel(int level, int niter = 1000);

private:
    Game* m_game;
};

#endif // BENCHMARKSUITE_H
