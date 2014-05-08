#include "benchmarksuite.h"

BenchmarkSuite::BenchmarkSuite(Game* game) : m_game(game){}

void BenchmarkResults::print(ostream& fout){

    fout <<"######### RESULTS ############" << endl;
    fout << "ProbWin  " << probWin.mean() << endl;
    fout << "AveMoves  " << aveMoves.mean() << " " << aveMoves.stdDev() << endl;
    fout << "AvePoints  " << avePoints.mean() << " " << avePoints.stdDev() << endl;

}

BenchmarkResults BenchmarkSuite::testLevel(int level, int niter, int seed){
    Player player(m_game, seed);
    BenchmarkResults res;
    res.niter = niter;
    res.level = level;
    m_game->setLevel(level);
//        m_game->printParams();
    for(int i = 0; i < niter; i++){
        m_game->startNewGame();
        cout << "PARTITA   " << i << endl;
        m_game->printBoard();


        int step = 0;
        while(!m_game->isFinished()){
//            m_game->printState();
            player.playRandomMove(true); //verbose
            if(!m_game->isWon()){
                step++;
            }
            m_game->printBoard();
        }
        res.probWin += m_game->isWon() ? 1 : 0;
        res.aveMoves += step;
        res.avePoints += m_game->points();
    }

    return res;
}


void BenchmarkSuite::singleGame(double qi,  int seedPlayer, bool verbose){
    Player* player = new Player(m_game, seedPlayer);
    m_game->setLevel(0);  //from 0 to 4
    m_game->startNewGame();
    if(verbose){
        m_game->printParams();
        m_game->printState();
        m_game->printBoard();
    }
    while(!m_game->isFinished()){
        if(verbose){
            cout << "numero di mosse possibili: " << m_game->availMoves().size() << endl;
            cout << endl;
            m_game->printMoves();
        }
        player->playSmartRandomMove(qi, verbose);
        if(verbose){
            m_game->printBoard();
            m_game->printState();
        }
    }

}


