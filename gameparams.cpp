#include "gameparams.h"

GameParams::GameParams(){
    setLevel(0);
}

GameParams::~GameParams(){
    //dtor
}

int GameParams::level() const {
    return m_level;
}

int GameParams::moves() const {
    return m_moves;
}

int GameParams::points() const {
    return m_points;
}

int GameParams::boardSize() const {
    return m_size;
}


int GameParams::colorCount() const {
    return m_colors;
}


int GameParams::mask() const {
    return m_mask;
}

bool GameParams::isDiamGenBiased() const {
    return m_isDiamGenBiased;
}


double GameParams::biasDiamGen() const{
    return m_biasDiamGen;
}

void GameParams::setLevel(int level){
    ifstream fparams(m_paramsPath);

    string line;
    for(int i = 0; i <= level; i++){
        getline(fparams, line);
        cout << line << endl;
    }
    stringstream stream(line);

    m_level = level;
    stream >> m_mask;
    stream >> m_colors;
    stream >> m_moves;
    stream >> m_points;

    //TODO inserire anche questi nel file
    m_isDiamGenBiased = isDiamondGeneratorBiased[level];
    m_biasDiamGen = biasDiamondGenerator[level];
}
