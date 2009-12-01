#include "LineShape.hxx"

#include <logging/logging.hxx>
#include <algorithm>

namespace DBricks {

LineShape::LineShape(const Point& from, const Point& to)
    :m_fhandle("from", this, Point(0, 0)),
     m_thandle("to", this, Point(0, 0)),
     m_fconnector(this, Point(0, 0)),
     m_tconnector(this, Point(0, 0))
{
    m_corner.x = std::min(from.x, to.x);
    m_corner.y = std::min(from.y, to.y);

    m_fhandle.point(from - m_corner);
    m_thandle.point(to - m_corner);

    m_handles.push_back(&m_fhandle);
    m_handles.push_back(&m_thandle);

    m_connectors.push_back(&m_fconnector);
    m_connectors.push_back(&m_tconnector);
}

void
LineShape::draw_shape (Cairo::RefPtr<Cairo::Context> ctx) const
{
    Point from(m_fhandle.point()+m_corner);
    Point to(m_thandle.point()+m_corner);
    
    ctx->save();
    ctx->move_to(from.x, from.y);
    ctx->line_to(to.x, to.y);
    ctx->restore();
}

void
LineShape::move_handle(Handle* handle, const Point& delta)
{
    if (handle == &m_fhandle) {
        m_fhandle.point(m_fhandle.point()+delta);
    } else if (handle == &m_thandle) {
        m_thandle.point(m_thandle.point()+delta);
    } else {
        ASSERT_NOT_REACHED();
    }

    Point from(m_fhandle.point() + m_corner);
    Point to(m_thandle.point() + m_corner);

    m_corner.x = std::min(from.x, to.x);
    m_corner.y = std::min(from.y, to.y);

    m_fhandle.point(from - m_corner);
    m_thandle.point(to - m_corner);

    m_fconnector.point(from - m_corner);
    m_tconnector.point(to - m_corner);
}

void
LineShape::move_connector(Connector* connector, const Point& delta)
{
    if (connector == &m_fconnector) {
        m_fconnector.point(m_fconnector.point()+delta);
    } else if (connector == &m_tconnector) {
        m_tconnector.point(m_tconnector.point()+delta);
    } else {
        ASSERT_NOT_REACHED();
    }

    Point from(m_fconnector.point() + m_corner);
    Point to(m_tconnector.point() + m_corner);

    m_corner.x = std::min(from.x, to.x);
    m_corner.y = std::min(from.y, to.y);

    m_fhandle.point(from - m_corner);
    m_thandle.point(to - m_corner);

    m_fconnector.point(from - m_corner);
    m_tconnector.point(to - m_corner);
}

void
LineShape::move(const Point& delta)
{
    m_corner += delta;
}

bool
LineShape::in(const Rect& rect) const
{
    Point from(m_corner + m_fhandle.point());
    Point to(m_corner + m_thandle.point());

    return
        from.x >= rect.x1() && to.x >= rect.x1() &&
        from.y >= rect.y1() && to.y >= rect.y1() &&
        from.x <= rect.x2() && to.x <= rect.x2() &&
        from.y <= rect.y2() && to.y <= rect.y2();
}


bool
LineShape::cover(const Point& point) const
{
    return distance(point);
}

Rect
LineShape::bb() const
{
    Point from(m_fhandle.point() + m_corner);
    Point to(m_thandle.point() + m_corner);

    return Rect(from, to);
}

} // namespace DBricks
