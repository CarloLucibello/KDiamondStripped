#ifndef RANDOMCOLOR_H
#define RANDOMCOLOR_H

#include "rand.h"
#include <math.h>

enum class Color;

class Board;

class RandomColor{
public:
    RandomColor(Board * board);
    void setSeed(int seed);
    Color biased(int x, double bias);
    Color unif();
    void init();

private:
    LinCongRNG m_rng;
    int m_numColors;
    Board * m_board;
};

#endif // RANDOMCOLOR_H
