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
        m_game->startNewGame();
//        cout << "PARTITA   " << i << endl;
//        m_game->printBoard();


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

void BenchmarkSuite::singleGame(double qi, int numero_mosse, int seedPlayer){
    Player* player = new Player(m_game, seedPlayer);
    m_game->setLevel(0);  //from 0 to 4
    m_game->startNewGame();
    m_game->printBoard();

    for (int i=0; i<numero_mosse; ++i){
        cout << "numero di mosse possibili: " << m_game->availMoves().size() << endl;
        cout << endl;
        //game->printMoves();

        player->playSmartRandomMove(qi);
        cout << endl;

        m_game->printBoard();

    }

}


