#ifndef FIGURE_H
#define FIGURE_H

#include <vector>
#include "point.h"
using namespace std;

enum class FigureType {
    None,
    RowH = 1,
    RowV,
    LT,
    Bag2  // il secondo stadio della Bag, viene scoppiato automaticamente dopo che `e sceso
};

class Figure{
public:
    Figure();
    Figure(vector<Point> points, FigureType type);
    vector<Point> points() const;
    FigureType type() const;
    int size() const;
    bool empty() const;


    FigureType m_type;
    vector<Point> m_points;
};

#endif // FIGURE_H
