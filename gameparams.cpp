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

bool GameParams::isDiamGenBiased() const {
    return m_isDiamGenBiased;
}


double GameParams::biasDiamGen() const{
    return m_biasDiamGen;
}

void GameParams::setLevel(int level){
    m_level = level;
    m_size = boardSizes[level];
    m_colors = numColors[level];
    m_moves = numMoves[level];
    m_points = pointsToReach[level];
    m_isDiamGenBiased = isDiamondGeneratorBiased[level];
    m_biasDiamGen = biasDiamondGenerator[level];
}
