#include "LineShape.hxx"

#include "SML.hxx"

#include <geom/computation.hxx>
#include <logging/logging.hxx>
#include <algorithm>

namespace DBricks {

LineShape::LineShape(const Point& start, Handle*& handle)
    :Shape(Break_Connections)
{
    // TODO: better default value ...
    initialize(start, Point(start.x+5.0, start.y+5.0));
    handle = &m_fhandle;
}

LineShape::LineShape(const Point& from, const Point& to)
    :Shape(Break_Connections)
{
    initialize(from, to);
}

LineShape::LineShape()
    :Shape(Break_Connections)
{
}

void
LineShape::initialize(const Point& from, const Point& to)
{
    m_fconnector.initialize(this, from, Connector::Active);
    m_tconnector.initialize(this, to,   Connector::Active);

    m_fhandle.initialize(this, &m_fconnector, from);
    m_thandle.initialize(this, &m_tconnector, to);

    m_corner.x = std::min(from.x, to.x);
    m_corner.y = std::min(from.y, to.y);

    m_handles.push_back(&m_fhandle);
    m_handles.push_back(&m_thandle);

    m_connectors.push_back(&m_fconnector);
    m_connectors.push_back(&m_tconnector);    
}

void
LineShape::draw_shape (Cairo::RefPtr<Cairo::Context> ctx) const
{
    Point from(m_fhandle.point());
    Point to(m_thandle.point());
    
    ctx->save();
    ctx->move_to(from.x, from.y);
    ctx->line_to(to.x, to.y);
    ctx->stroke();
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

    Point from(m_fhandle.point());
    Point to(m_thandle.point());

    m_corner.x = std::min(from.x, to.x);
    m_corner.y = std::min(from.y, to.y);

    m_fconnector.point(from);
    m_tconnector.point(to);
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

    Point from(m_fconnector.point());
    Point to(m_tconnector.point());

    m_corner.x = std::min(from.x, to.x);
    m_corner.y = std::min(from.y, to.y);

    m_fhandle.point(from);
    m_thandle.point(to);
}

void
LineShape::move(const Point& delta)
{
    m_corner += delta;
    m_fhandle.point(m_fhandle.point() + delta);
    m_thandle.point(m_thandle.point() + delta);

    m_fconnector.point(m_fconnector.point() + delta);
    m_tconnector.point(m_tconnector.point() + delta);
}

bool
LineShape::in(const Rect& rect) const
{
    Point from(m_fhandle.point());
    Point to(m_thandle.point());

    return
        from.x >= rect.x1() && to.x >= rect.x1() &&
        from.y >= rect.y1() && to.y >= rect.y1() &&
        from.x <= rect.x2() && to.x <= rect.x2() &&
        from.y <= rect.y2() && to.y <= rect.y2();
}


double
LineShape::distance(const Point& point) const
{
    return point_line_distance(point, m_fhandle.point(), m_thandle.point());
}

bool
LineShape::cover(const Point& point) const
{
    Rect r = bb();
    return distance(point) < 50 &&
        point.x >= r.x1() && point.y >= r.y1() &&
        point.x <= r.x2() && point.y <= r.y2();
}

Rect
LineShape::bb() const
{
    return Rect(m_fhandle.point(), m_thandle.point());
}

void
LineShape::save(Sml::Object* object) const
{
    object->add_attribute_data("name", "Line");
    object->add_attribute_data("type", "Line");
    
    object->add_attribute_data("fx", m_fhandle.point().x);
    object->add_attribute_data("fy", m_fhandle.point().y);
    object->add_attribute_data("tx", m_thandle.point().x);
    object->add_attribute_data("ty", m_thandle.point().y);
}



void
LineShape::load(Sml::Object* object)
{
    Point from;
    Point to;

    object->get_attribute_data("fx", from.x);
    object->get_attribute_data("fy", from.y);
    object->get_attribute_data("tx", to.x);
    object->get_attribute_data("ty", to.y);
    
    initialize(from, to);
}


} // namespace DBricks
