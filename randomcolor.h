#ifndef RANDOMCOLOR_H
#define RANDOMCOLOR_H

#include "rand.h"

enum class Color;

class Board;

class RandomColor{
public:
    RandomColor(Board * board);
    void setSeed(int seed);
//    void copyStateRNG(const RandomColor * randcol);
    Color biased(int x, double bias);
    Color unif();
    void init();
    RandomColor& operator=(const RandomColor rcol);

private:
    LinCongRNG m_rng;
    int m_numColors;
    Board * m_board;
};

#endif // RANDOMCOLOR_H
