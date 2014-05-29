#include "point.h"

Point::Point()
    : m_x(0)
    , m_y(0){}


Point::Point(int x, int y)
    : m_x(x)
    , m_y(y){}

int Point::x() const {
    return m_x;
}

int Point::y() const {
    return m_y;
}


int& Point::rx(){
    return m_x;
}

int& Point::ry(){
    return m_y;
}

Point& Point::operator+=(const Point& p){
    m_x += p.m_x;
    m_y += p.m_y;

    return *this;
}


bool operator==(const Point& p1, const Point& p2){
    return (p1.x() == p2.x() &&
            p1.y() == p2.y());
}

bool operator!= (const Point& p1, const Point& p2){
    return !(p1 == p2);
}


Point operator+(const Point& p1, const Point& p2){
    Point newp = p1;
    newp += p2;
    return newp;
}
