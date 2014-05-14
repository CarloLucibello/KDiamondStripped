#ifndef MASK_H
#define MASK_H

#include <string>
#include <iostream>
#include <fstream>
#include <QVector>
#include <QPoint>
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
    CellMask& rCell(const QPoint& point);
    CellMask cell(const QPoint& point) const;


    QVector<CellMask> m_cells;
    const int m_size = 9; //ATTENZIONE in futuro andrà estratto dal file
};

#endif // MASK_H
