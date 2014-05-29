#ifndef POINT_H
#define POINT_H

class Point{
public:
    Point();
    Point(int x, int y);
    int x() const;
    int y() const ;
    int& rx();
    int& ry();
    Point& operator+=(const Point& p);


private:
    int m_x;
    int m_y;
};

bool operator==(const Point& p1, const Point& p2);
bool operator!= (const Point& p1, const Point& p2);
Point operator+(const Point& p1, const Point& p2);

#endif // POINT_H
