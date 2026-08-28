#include "Point.h"

Point::Point()
{
    id = -1;
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

Point::Point(int id_, double x_, double y_)
{
    id = id_;
    x = x_;
    y = y_;
    z = 0.0;
}

void Point::print() const
{
    std::cout
        << "Point "
        << id
        << " : ("
        << x
        << ", "
        << y
        << ")"
        << std::endl;
}
