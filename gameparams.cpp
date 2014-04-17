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

int GameParams::boardSize() const {
    return m_size;
}


int GameParams::colorCount() const {
    return m_colorCount;
}

void GameParams::setLevel(const int level){
    m_level = level;
    m_size = boardSizes[level];
    m_colorCount = boardColorCounts[level];
    m_moves = numMoves[level];
}
