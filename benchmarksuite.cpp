#include "benchmarksuite.h"

BenchmarkSuite::BenchmarkSuite(Game* game) : m_game(game){}

void BenchmarkSuite::testParam( string paramName, string levelPath, string expPath){
    
    int m_colors;
    int m_mask;
    int m_level;
    int m_moves;
    int m_points;
    bool m_isDiamGenBiased;
    double m_biasDiamGen;
    bool m_targetLiquirizia;
    ifstream fparams(levelPath);
    ofstream fout(expPath);
    
    string line;
    for(int i = 0; i <= 1; i++){
        getline(fparams, line);
    }
    stringstream stream(line);
    
    m_level = 1;
    stream >> m_mask;
    stream >> m_colors;
    stream >> m_moves;
    stream >> m_points;
    stream >> m_isDiamGenBiased;
    stream >> m_biasDiamGen;
    stream >> m_targetLiquirizia;
    
    if (paramName== "mask") {
        int k;
        int kMax = 18;
        fout << "### $1=Mask $2=NumColors $3=NumMoves $4=PointsToReach  $5=IsBiased  $6=Bias  $7=TargetLiquirizia" << endl;
        for (k=0; k<= kMax; k++) {
            fout << m_mask + k << "\t" << m_colors << "\t" << m_moves << "\t" << m_points << "\t" \
            << m_isDiamGenBiased << "\t" << m_biasDiamGen << "\t"<< m_targetLiquirizia << endl;
        }
        fout.close();
    }
    
}

void BenchmarkSuite::fullTestLevel( double qi, int niter, int seed, bool verbose, string paramsPath, string outPath){
    
    int level;
    int numLines = 0;
    string unused;
    string outParamPath = "simParam.txt";
    ifstream fin(paramsPath);
    ofstream fout(outPath);
    ofstream foutParam(outParamPath);
    
    foutParam << "qi \t niter \t seed" << endl;
    foutParam << qi << "\t" << niter << "\t" << seed << "\t" << endl;
    foutParam.close();
    
    while ( std::getline(fin, unused) )  ++numLines;
    

    for (level=1; level< numLines; ++level) {
        cout << "line " << level << endl ;
        auto res = testLevel(level, qi, niter, seed, verbose, paramsPath);
        res.print(fout);

    }
   fout.close();
}

void BenchmarkResults::print(ostream& fout){

    fout <<  fixed << setprecision(3) << showpoint << probWin.mean()  << "\t" << \
    aveMoves.mean() <<  "\t" << aveMoves.stdDev()  << \
    avePoints.mean() <<  "\t" << avePoints.stdDev() << endl;

}

BenchmarkResults BenchmarkSuite::testLevel(int level, double qi, int niter, int seed, bool verbose, string paramsPath){
    Player player(m_game, seed);
    BenchmarkResults res;
    res.niter = niter;
    res.level = level;

    m_game->setParamsPath(paramsPath);
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
            player.playFuzzyMove(qi);
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

        if(verbose){
            cout << "numero di mosse possibili: " << m_game->availMoves().size() << endl;
            cout << endl;
            m_game->printMoves();
        }
        
        player->playFuzzyMove(qi, verbose);
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

    player->playFuzzyMove(qi, verbose);
    if(verbose){
        m_game->printBoard();
        m_game->printState();
    }
}


