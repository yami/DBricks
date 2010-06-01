#ifndef POINT_HXX
#define POINT_HXX

#include <cmath>

namespace DBricks {

struct Point {
    Point()
    {
    }
    
    Point(double xx, double yy)
        :x(xx), y(yy)
    {
    }

    Point(const Point& p)
        :x(p.x), y(p.y)
    {
    }
    
    Point operator + (const Point& p) const
    {
        return Point(x+p.x, y+p.y);
    }
    
    Point& operator += (const Point& p)
    {
        x += p.x;
        y += p.y;

        return *this;
    }

    Point operator - (const Point& p) const
    {
        return Point(x-p.x, y-p.y);
    }

    Point& operator -= (const Point& p)
    {
        x -= p.x;
        y -= p.y;

        return *this;
    }

    bool operator == (const Point& p) const
    {
        return std::fabs(x - p.x) + std::fabs(y - p.y) < 0.001;
    }
    
    double x, y;
};

} // namespace DBricks

#endif  // POINT_HXX
