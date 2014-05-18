#include "mask.h"

Mask::Mask(){}

void Mask::set(int nmask){
    string maskPath("levels/masks/mask-");
    maskPath += to_string(nmask) + ".txt";
    ifstream fmask(maskPath);
    m_cells.resize(m_size * m_size);
    for(int y = 0; y < m_size; ++y){
        for(int x = 0; x < m_size; ++x){
            int m;
            fmask >> m;
            rCell({x, y}) = CellMask(m);
        }
    }
    fmask.close();
}

CellMask& Mask::rCell(const QPoint& point){
    return m_cells[point.x() + point.y() * m_size];
}


CellMask Mask::cell(const QPoint& point) const {
    return m_cells[point.x() + point.y() * m_size];
}

Mask& Mask::operator=(const Mask& mask){
    m_cells = mask.m_cells;
    return *this;
}

