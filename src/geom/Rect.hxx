#ifndef RECT_HXX
#define RECT_HXX

#include <algorithm>

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
private:
    double m_x;
    double m_y;
    double m_width;
    double m_height;
};

} // namespace DBricks

#endif // RECT_HXX
