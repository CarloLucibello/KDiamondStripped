#include "gameparams.h"

GameParams::GameParams(){
    setLevel(0);
}

GameParams::~GameParams(){
    //dtor
}

int GameParams::level(){
    return m_level;
}

int GameParams::boardSize(){
    return m_size;
}


int GameParams::colorCount(){
    return m_colorCount;
}

void GameParams::setLevel(const int& level){
    m_level = level;
    m_size = boardSizes[level];
    m_colorCount = boardColorCounts[level];
}
