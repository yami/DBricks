#include "BoxShape.hxx"

#include <util/assert.hxx>

namespace DBricks {

BoxShape::BoxShape(const Rect& rect)
    :Shape(rect.x1(), rect.y1()), m_x(m_corner.x), m_y(m_corner.y),
     m_width(rect.width()), m_height(rect.height()),
     m_lhandle("left", this, Point(0, rect.height()/2)),
     m_rhandle("right", this, Point(rect.width(), rect.height()/2)),
     m_thandle("top", this, Point(rect.width()/2, 0)),
     m_bhandle("bottom", this, Point(rect.width()/2, rect.height()))
{
    m_handles.push_back(&m_lhandle);
    m_handles.push_back(&m_rhandle);
    m_handles.push_back(&m_thandle);
    m_handles.push_back(&m_bhandle);    
}

BoxShape::BoxShape(double x, double y, double width, double height)
    :Shape(x, y), m_x(m_corner.x), m_y(m_corner.y),
     m_width(width), m_height(height),
     m_lhandle("left", this, Point(0, height/2)),
     m_rhandle("right", this, Point(width, height/2)),
     m_thandle("top", this, Point(width/2, 0)),
     m_bhandle("bottom", this, Point(width/2, height))
{
    m_handles.push_back(&m_lhandle);
    m_handles.push_back(&m_rhandle);
    m_handles.push_back(&m_thandle);
    m_handles.push_back(&m_bhandle);
}


void
BoxShape::update_handles()
{
    m_lhandle.point(Point(0         , m_height/2));
    m_rhandle.point(Point(m_width   , m_height/2));
    m_thandle.point(Point(m_width/2 , 0));
    m_bhandle.point(Point(m_width/2 , m_height));
}

void
BoxShape::move_handle(Handle* handle, const Point& delta)
{
    if (handle == &m_lhandle) {
        m_x     += delta.x;
        m_width -= delta.x;
    } else if (handle == &m_rhandle) {
        m_width += delta.x;
    } else if (handle == &m_thandle) {
        m_y      += delta.y;
        m_height -= delta.y;
    } else if (handle == &m_bhandle) {
        m_height += delta.y;
    } else {
        ASSERT_NOT_REACHED();
    }

    update_handles();
}

void
BoxShape::move(const Point& delta)
{
    m_corner += delta;
}

bool
BoxShape::cover(const Point& point) const
{
    return point.x >= m_x && point.x <= m_x + m_width &&
        point.y >= m_y && point.y <= m_y + m_height;
}

bool
BoxShape::in(const Rect& rect) const
{
    return
        m_corner.x >= rect.x1() &&
        m_corner.y >= rect.y1() &&
        (m_corner.x + m_width) <= rect.x2() &&
        (m_corner.y + m_height) <= rect.y2();
}

Rect
BoxShape::bb() const
{
    return Rect(m_corner, m_width, m_height);
}

} // namespace DBricks
