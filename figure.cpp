#include "figure.h"

Figure::Figure() {}

Figure::Figure(vector<Point> points, FigureType type)
    : m_points(points)
    , m_type(type) {}

vector<Point> Figure::points() const{
    return m_points;
}

FigureType Figure::type() const {
    return m_type;
}

int Figure::size() const {
    return m_points.size();
}

bool Figure::empty() const {
    return m_points.empty();
}
