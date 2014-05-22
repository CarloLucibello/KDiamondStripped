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

    int m_colorMin = 3;
    int m_colorMax = 5;
    int m_maskMin = 1;
    int m_maskMax = 19;
    int m_movesMin = 5;
    int m_movesMax = 20;
    int m_pointsMin = 60;
    int m_pointsMax = 80;
    BenchmarkSuite(Game* game);
    BenchmarkResults testLevel(int level, double qi, int niter = 1000, int seed = -1, bool verbose = false, string paramsPath = "levels/params.txt");
    void singleGame(int level, double qi, int seedPlayer = -1, bool verbose = false);  //verbose si potrebbe eliminare da qua
    void singleMove(int level, double qi, int seedPlayer = -1, bool verbose = false);  //verbose si potrebbe eliminare da qua
    
    void fullTestLevel( double qi, int niter, int seed, bool verbose, string paramsPath, string outPath);

    void testParam(string paramName, string levelPath, string expPath);
    void fullTestParam(double qi, int niter, int seed, bool verbose);

private:
    Game * m_game;
};

#endif // BENCHMARKSUITE_H
