#include "benchmarksuite.h"

BenchmarkSuite::BenchmarkSuite(Game* game) : m_game(game){}

void BenchmarkSuite::fullTestParam(double qi, int niter, int seed, bool verbose){
    
    const char* const listParams[] = { "color", "moves", "points"};
    const int len = 3;
    
    string unused;
    ifstream fin("levels/params.txt");
    
    string currentParam;
    string paramsPath;
    string outPath;
    
    int numLines = 0;
    while ( std::getline(fin, unused) )  ++numLines;
    
    cout << numLines  << endl;
    
    for (int level = 1; level < numLines; ++level ){
        
        stringstream ss;
        ss << level;
        string str = ss.str();
        
        for (int i = 0; i < len; ++i) {
        currentParam = listParams[i];
        paramsPath = "l" + str + currentParam  + "Params.txt";
        outPath = "l" + str + currentParam  + "Res.txt";
            
        cout << str << endl;
        cout << currentParam << endl ;
        testParam( listParams[i], "levels/params.txt", paramsPath, level);
        fullTestLevel( qi, niter, seed, verbose, paramsPath, outPath);
        
        }
    }
    
}

void BenchmarkSuite::testParam( string paramName, string levelPath, string expPath, int k){
    
    int colors;
    int mask;
    int level;
    int moves;
    int points;
    bool isDiamGenBiased;
    double biasDiamGen;
    bool targetLiquirizia;
    ifstream fparams(levelPath);
    ofstream fout(expPath);
    
    
    string line;
    for(int i = 0; i <= k; i++){
        getline(fparams, line);
    }
    stringstream stream(line);
    
    level = k;
    stream >> mask;
    stream >> colors;
    stream >> moves;
    stream >> points;
    stream >> isDiamGenBiased;
    stream >> biasDiamGen;
    stream >> targetLiquirizia;
    
    cout << colors << endl;
    cout << mask << endl;
    cout << moves << endl;
    
    if (paramName== "mask") {
        
        fout << "### $1=Mask $2=NumColors $3=NumMoves $4=PointsToReach  $5=IsBiased  $6=Bias  $7=TargetLiquirizia" << endl;
        for (int k = m_maskMin; k <= m_maskMax; k += m_maskStep) {
    
            fout << k << "\t" << colors << "\t" << moves << "\t" << points << "\t" \
            << isDiamGenBiased << "\t" << biasDiamGen << "\t"<< targetLiquirizia << endl;
        }
        fout.close();
    }
    
    if (paramName== "color") {
        
        fout << "### $1=Mask $2=NumColors $3=NumMoves $4=PointsToReach  $5=IsBiased  $6=Bias  $7=TargetLiquirizia" << endl;
        for (int k=m_colorMin; k<= m_colorMax; k += m_colorStep) {
            
            fout << mask << "\t" << k << "\t" << moves << "\t" << points << "\t" \
            << isDiamGenBiased << "\t" << biasDiamGen << "\t"<< targetLiquirizia << endl;
        }
        fout.close();
    }
    
    if (paramName== "moves") {

        fout << "### $1=Mask $2=NumColors $3=NumMoves $4=PointsToReach  $5=IsBiased  $6=Bias  $7=TargetLiquirizia" << endl;
        for (int k=m_movesMin; k<= m_movesMax; k += m_movesStep) {
            
            fout << mask  << "\t" << colors << "\t" << k << "\t" << points << "\t" \
            << isDiamGenBiased << "\t" << biasDiamGen << "\t"<< targetLiquirizia << endl;
        }
        fout.close();
    }
    
    if (paramName== "points") {
        
        fout << "### $1=Mask $2=NumColors $3=NumMoves $4=PointsToReach  $5=IsBiased  $6=Bias  $7=TargetLiquirizia" << endl;
        for (int k=m_pointsMin; k<= m_pointsMax; k += m_pointsStep) {
            
            fout << mask  << "\t" << colors << "\t" << moves  << "\t" << k << "\t" \
            << isDiamGenBiased << "\t" << biasDiamGen << "\t"<< targetLiquirizia << endl;
        }
        fout.close();
    }
    
    if (paramName== "bias") {
        
        fout << "### $1=Mask $2=NumColors $3=NumMoves $4=PointsToReach  $5=IsBiased  $6=Bias  $7=TargetLiquirizia" << endl;
        for (double k=m_biasMin; k<= m_biasMax; k += m_biasStep) {
            
            fout << mask  << "\t" << colors << "\t" << moves  << "\t" << points << "\t" \
            << isDiamGenBiased << "\t" << k << "\t"<< targetLiquirizia << endl;
        }
        fout.close();
    }
}

void BenchmarkSuite::fullTestLevel( double qi, int niter, int seed, bool verbose, string paramsPath, string outPath){
    

    string unused;
    string outParamPath = "simParam.txt";
    ifstream fin(paramsPath);
    ofstream fout(outPath);
    ofstream foutParam(outParamPath);
    
    foutParam << "qi \t niter \t seed" << endl;
    foutParam << qi << "\t" << niter << "\t" << seed << "\t" << endl;
    foutParam.close();
    
    int numLines = 0;
    while ( std::getline(fin, unused) )  ++numLines;

    for (int level=1; level< numLines; ++level) {
        cout << "line " << level << endl ;
        auto res = testLevel(level, qi, niter, seed, verbose, paramsPath, fout);

    }
   fout.close();
}

void BenchmarkResults::print(ostream& fout){

    
   
    fout    << level  << " \t"
            << niter  << " \t"
            << points.mean() << " \t"
//            << moves.mean()  << " \t"
            << isWon.mean()  << endl;
}

BenchmarkResults BenchmarkSuite::testLevel(int level, double qi, int niter, int seed, bool verbose, string paramsPath, ostream& fout){
    Player player(m_game, seed);
    BenchmarkResults res;
    
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
        res.isWon = m_game->isWon() ? 1 : 0;
        res.moves = step;
        res.points = m_game->points();
        res.niter = i+1;
        res.print(fout);
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
