#ifndef RANDOMCOLOR_H
#define RANDOMCOLOR_H

#include "rand.h"

enum class Color;

class Board;

class RandomColor{
public:
    RandomColor(Board * board, bool isBiased = false, double bias = 0);
    void setSeed(int seed);
    void copyStateRNG(const RandomColor * randcol);
    Color gen(int x);

private:
    LinCongRNG m_rng;
    int m_numColors;
    bool m_isBiased;
    double m_bias;
    Board * m_board;
};

#endif // RANDOMCOLOR_H
