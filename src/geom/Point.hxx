#ifndef POINT_HXX
#define POINT_HXX

namespace DBricks {

struct Point {
    Point()
    {
    }
    
    Point(double xx, double yy)
        :x(xx), y(yy)
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
    
    double x, y;
};

} // namespace DBricks

#endif  // POINT_HXX
