#include "benchmarksuite.h"

BenchmarkSuite::BenchmarkSuite(Game* game) : m_game(game){}

void BenchmarkResults::print(ostream& fout){

    fout << "ProbWin  " << probWin.mean() << endl;
    fout << "AveMoves  " << aveMoves.mean() << " " << aveMoves.stdDev() << endl;
    fout << "AvePoints  " << avePoints.mean() << " " << avePoints.stdDev() << endl;

}

BenchmarkResults BenchmarkSuite::testLevel(int level, int niter, int seed, bool verbose){
    Player player(m_game, seed);
    BenchmarkResults res;
    res.niter = niter;
    res.level = level;
    m_game->setLevel(level);
//        m_game->printParams();
    for(int i = 0; i < niter; i++){
        m_game->startNewGame();
        if(verbose){
            cout << "PARTITA   " << i << endl;
        }

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

    return res;
}


void BenchmarkSuite::singleGame(int level, double qi,  int seedPlayer, bool verbose){
    Player* player = new Player(m_game, seedPlayer);
    m_game->setLevel(level);  //from 1 to N
    m_game->printParams();
    m_game->startNewGame();

    while(!m_game->isFinished()){

        player->playSmartRandomMove(qi, verbose);
        if(verbose){
            m_game->printBoard();
            m_game->printState();
        }
    }

}

void BenchmarkSuite::singleMove(int level, double qi,  int seedPlayer, bool verbose){
    Player* player = new Player(m_game, seedPlayer);
    m_game->setLevel(level);  //from 1 to N
    m_game->printParams();
    m_game->startNewGame();

    player->playSmartRandomMove(qi, verbose);
    if(verbose){
        m_game->printBoard();
        m_game->printState();
    }
}


