#ifndef MASK_H
#define MASK_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "point.h"
using namespace  std;


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
    const int m_size = 9; //ATTENZIONE in futuro andrà estratto dal file
};

#endif // MASK_H
