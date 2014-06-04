#ifndef MASK_H
#define MASK_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "point.h"
using namespace  std;

// alla matrice quadrata di lato m_size sovrapponiamo una maschera che
// da un valore di CellMask ad ogni punto. Se la cella è una cella normale
// questo valore è BLANK, se è una cella impenetreabile questo valore è WALL
// e in modo analogo per GELATINA E LIQUIRIZIA.
// il valore di una cella può cambiare durante il gioco: ad esempio quando
// scoppiamo una gelatina o una liquirizia.

enum class CellMask {
    BLANK,  //normal cell
    WALL,
    GELATINA,
    LIQUIRIZIA
};

class Mask{
public:
    Mask();

    void set(int nmask);
    CellMask& rCell(const Point& point);
    CellMask cell(const Point& point) const;
    Mask& operator=(const Mask& mask);

    vector<CellMask> m_cells;
    const int m_size = 9;
    //ATTENZIONE in futuro andrà estratto dal file delle maschere.
    //m_size è il lato della matrice, che per ora è fissato a 9 e non è
    //uno dei parametri che si possono scegliere dal file
    //levels/params.
    
};

#endif // MASK_H
