#include "figure.h"

Figure::Figure() {}

Figure::Figure(QVector<QPoint> points, FigureType type)
    : m_points(points)
    , m_type(type) {}

QVector<QPoint> Figure::points() const{
    return m_points;
}

FigureType Figure::type() const {
    return m_type;
}

int Figure::size() const {
    return m_points.size();
}

bool Figure::isEmpty() const {
    return m_points.isEmpty();
}
