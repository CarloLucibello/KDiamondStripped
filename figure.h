#ifndef FIGURE_H
#define FIGURE_H

#include <QVector>
#include <QPoint>

enum class FigureType {
    None,
    RowH = 1,
    RowV,
    LT
};

class Figure{
public:
    Figure();
    Figure(QVector<QPoint> points, FigureType type);
    QVector<QPoint> points() const;
    FigureType type() const;
    int size() const;
    bool isEmpty() const;


    FigureType m_type;
    QVector<QPoint> m_points;
};

#endif // FIGURE_H
