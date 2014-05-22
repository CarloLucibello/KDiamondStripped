#include "benchmarksuite.h"

BenchmarkSuite::BenchmarkSuite(Game* game) : m_game(game){}

void BenchmarkSuite::fullTestParam(double qi, int niter, int seed, bool verbose){
    
    const char* const listParams[] = {"mask", "color", "moves", "points"};
    const int len = 4;
    int i;
    
    for (i = 0; i < len; ++i) {
        string currentParam = listParams[i];
        string paramsPath = currentParam  + "Params.txt";
        string outPath = currentParam  + "Res.txt";
        
        cout << currentParam << endl ;
        testParam( listParams[i], "levels/params.txt", paramsPath);
        fullTestLevel( qi, niter, seed, verbose, paramsPath, outPath);
        
    }
    
}

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
        int paramVal;
        
        fout << "### $1=Mask $2=NumColors $3=NumMoves $4=PointsToReach  $5=IsBiased  $6=Bias  $7=TargetLiquirizia" << endl;
        for (k=m_maskMin; k<= m_maskMax; k++) {
            
            paramVal =  m_maskMin + static_cast<int>(k*(m_maskMax-m_maskMin)/m_maskMax);
            
            fout << paramVal << "\t" << m_colors << "\t" << m_moves << "\t" << m_points << "\t" \
            << m_isDiamGenBiased << "\t" << m_biasDiamGen << "\t"<< m_targetLiquirizia << endl;
        }
        fout.close();
    }
    
    if (paramName== "color") {
        int k;
        int paramVal;
        
        fout << "### $1=Mask $2=NumColors $3=NumMoves $4=PointsToReach  $5=IsBiased  $6=Bias  $7=TargetLiquirizia" << endl;
        for (k=m_colorMin; k<= m_colorMax; k++) {
            
            paramVal = m_colorMin + static_cast<int>(k*(m_colorMax-m_colorMin)/m_colorMax);
            
            fout << m_mask << "\t" << paramVal << "\t" << m_moves << "\t" << m_points << "\t" \
            << m_isDiamGenBiased << "\t" << m_biasDiamGen << "\t"<< m_targetLiquirizia << endl;
        }
        fout.close();
        fout.close();
    }
    
    if (paramName== "moves") {
        int k;
        int paramVal;
        
        fout << "### $1=Mask $2=NumColors $3=NumMoves $4=PointsToReach  $5=IsBiased  $6=Bias  $7=TargetLiquirizia" << endl;
        for (k=m_movesMin; k<= m_movesMax; k++) {
            
            paramVal = m_movesMin + static_cast<int>(k*(m_movesMax-m_movesMin)/m_movesMax);
            
            fout << m_mask  << "\t" << m_colors << "\t" << paramVal << "\t" << m_points << "\t" \
            << m_isDiamGenBiased << "\t" << m_biasDiamGen << "\t"<< m_targetLiquirizia << endl;
        }
        fout.close();
    }
    
    if (paramName== "points") {
        int k;
        int paramVal;
        
        fout << "### $1=Mask $2=NumColors $3=NumMoves $4=PointsToReach  $5=IsBiased  $6=Bias  $7=TargetLiquirizia" << endl;
        for (k=m_pointsMin; k<= m_pointsMax; k++) {
            
            paramVal = m_pointsMin + static_cast<int>(k*(m_pointsMax-m_pointsMin)/m_pointsMax);
            
            fout << m_mask  << "\t" << m_colors << "\t" << m_moves  << "\t" << paramVal  << "\t" \
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


