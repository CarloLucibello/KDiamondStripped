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

void BenchmarkSuite::Singlegame(Game* game, double qi, int numero_mosse, LinCongRNG::UInt8 seed_player, LinCongRNG::UInt8 seed_board){
    
    Player* player = new Player(game, seed_player);
    
    game->setLevel(0);  //from 0 to 4
    game->startNewGame(seed_board);
    
    cout << "***BOARD*****" << endl;
    game->printBoard();
    cout << endl;

    
    for (int i=0; i<numero_mosse; ++i){
        game->getMoves();
        
        cout << "numero di mosse possibili: " << game->availMoves().size() << endl;
        //cout << "***MOVES*****" << endl;
        //game->printMoves();
        cout << endl;
        
        player->playRandomMove_smarter(qi);
        
        cout << endl;
        
        cout << "***BOARD*****" << endl;
        game->printBoard();
        cout << endl;

    }
    
}


