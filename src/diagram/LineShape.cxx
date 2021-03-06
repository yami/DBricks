#include "LineShape.hxx"

#include <sml/Sml.hxx>

#include "PropertyMap.hxx"
#include "PropertyDescriptor.hxx"
#include "IntProperties.hxx"
#include "IRenderer.hxx"

#include <geom/computation.hxx>
#include <logging/logging.hxx>
#include <algorithm>

#include "DiagramArchiver.hxx"

namespace DBricks {

DEFINE_SHAPE_TYPE(LineShape, Line_Shape_Type, "line");

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
LineShape::draw_shape (IRenderer* renderer) const
{
    renderer->draw_line(m_fhandle.point(), m_thandle.point());
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
LineShape::save(DiagramArchiver* ar) const
{
    Sml::Object* object = ar->object();
    
    object->add_attribute_data("fx", m_fhandle.point().x);
    object->add_attribute_data("fy", m_fhandle.point().y);
    object->add_attribute_data("tx", m_thandle.point().x);
    object->add_attribute_data("ty", m_thandle.point().y);

    Sml::List* connectors_list = new Sml::List();

    size_t iconn = 0;
    for (ConnectorsType::const_iterator iter = m_connectors.begin();
         iter != m_connectors.end();
         ++iter, ++iconn) {
        if (!(*iter)->is_connected())
            continue;
        
        Sml::Object* connector_object = new Sml::Object();

        Connector* to_connector = (*iter)->connector(0);
        Shape*     to_shape     = to_connector->shape();
        int        to_index     = to_shape->connector_index(to_connector);

        ASSERT(to_index >= 0);
        
        connector_object->add_attribute_data("handle", (int)iconn);
        connector_object->add_attribute_data("to_shape", (void *)to_shape);
        connector_object->add_attribute_data("to_connector", to_index);

        connectors_list->add_value(new Sml::Value(connector_object));
    }

    object->add_attribute_data(":connectors", connectors_list);
}



void
LineShape::load(DiagramArchiver* ar)
{
    Sml::Object* object = ar->object();
    Point from;
    Point to;

    void* id;
    object->get_attribute_data(":id", id);
    
    object->get_attribute_data("fx", from.x);
    object->get_attribute_data("fy", from.y);
    object->get_attribute_data("tx", to.x);
    object->get_attribute_data("ty", to.y);

    Sml::List* connectors_list;
    object->get_attribute_data(":connectors", connectors_list);

    for (Sml::List::ValuesType::const_iterator iter = connectors_list->values().begin();
         iter != connectors_list->values().end();
         ++iter) {
        int   ihandle;
        void* to_shape;
        int   to_index;

        Sml::Object* connector_object = (*iter)->get_object();
        
        connector_object->get_attribute_data("handle", ihandle);
        connector_object->get_attribute_data("to_shape", to_shape);
        connector_object->get_attribute_data("to_connector", to_index);

        ar->add_connection(id, ihandle, to_shape, to_index);
    }
    
    initialize(from, to);
}

void
LineShape::property_apply()
{
    initialize(m_from, m_to);
}



static const char* property_layout =
    "(table :rows 2 :cols 2"
    "    (property \"X1\" x1) (property \"Y1\" y1)"
    "    (property \"X2\" x2) (property \"Y2\" y2)"
    ")";


Gtk::Widget*
LineShape::property_widget()
{
    m_from = m_fhandle.point();
    m_to   = m_thandle.point();
    
    if (!m_property_descriptor) {
        m_property_map = new PropertyMap(this);

        m_property_map->add(new NumberProperty<double>("X1", m_from.x, 0, 400));
        m_property_map->add(new NumberProperty<double>("Y1", m_from.y, 0, 400));
        m_property_map->add(new NumberProperty<double>("X2", m_to.x, 0, 400));
        m_property_map->add(new NumberProperty<double>("Y2", m_to.y, 0, 400));

        m_property_descriptor = new PropertyDescriptor(*m_property_map, property_layout);
    }

    return m_property_descriptor->to_widget();
}

ShapeType* LineShape::type() const
{
    return Line_Shape_Type;
}

} // namespace DBricks
