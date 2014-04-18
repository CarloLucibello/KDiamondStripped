#include "benchmarksuite.h"

BenchmarkSuite::BenchmarkSuite(Game* game) : m_game(game){}

BenchmarkResults BenchmarkSuite::testLevel(int level, int niter){
    Player player(m_game);
    BenchmarkResults res;
    res.niter = niter;
    res.level = level;
    m_game->setLevel(level);
//        m_game->printParams();
    for(int i = 0; i < niter; i++){

        m_game->startNewGame(); //random seed
        cout << "PARTITA   " << i << endl;
        m_game->printBoard();


        int step = 0;
        while(!m_game->isFinished()){
//            m_game->printState();
            player.playRandomMove();
            if(!m_game->isWon()){
                step++;
            }
        }
        res.probWin += m_game->isWon() ? 1 : 0;
        res.aveMoves += step;
        res.avePoints += m_game->points();
    }
    res.probWin /= niter;
    res.aveMoves /= niter;
    res.avePoints /= niter;

    return res;
}



