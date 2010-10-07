#ifndef RECT_HXX
#define RECT_HXX

#include <algorithm>
#include "computation.hxx"

namespace DBricks {

class Rect {
public:
    Rect(const Point& a, const Point& b)
    {
        m_x = std::min(a.x, b.x);
        m_y = std::min(a.y, b.y);
        m_width  = std::max(a.x, b.x) - m_x;
        m_height = std::max(a.y, b.y) - m_y;
    }

    Rect(double x1, double y1, double x2, double y2)
    {
        m_x = std::min(x1, x2);
        m_y = std::min(y1, y2);
        m_width  = std::max(x1, x2) - m_x;
        m_height = std::max(y1, y2) - m_y;
    }

    Rect(const Point& left_top, double width, double height)
        :m_x(left_top.x), m_y(left_top.y), m_width(width), m_height(height)
    {
    }
    

    Rect(const Rect& rect)
        :m_x(rect.m_x), m_y(rect.m_y), m_width(rect.m_width), m_height(rect.m_height)
    {
    }
    
    double x1() const
    {
        return m_x;
    }

    double y1() const
    {
        return m_y;
    }

    double x2() const
    {
        return m_x + m_width - 1;
    }

    double y2() const
    {
        return m_y + m_height -1 ;
    }

    double width() const
    {
        return m_width;
    }

    double height() const
    {
        return m_height;
    }

    bool operator == (const Rect& other) const
    {
        return
            double_equal(m_x, other.m_x) &&
            double_equal(m_y, other.m_y) &&
            double_equal(m_width, other.m_width) &&
            double_equal(m_height, other.m_height);
    }

    bool operator != (const Rect& other) const
    {
        return !(*this == other);
    }
    
private:
    double m_x;
    double m_y;
    double m_width;
    double m_height;
};

} // namespace DBricks

#endif // RECT_HXX
