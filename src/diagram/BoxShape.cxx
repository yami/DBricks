#include "BoxShape.hxx"

#include "Menu.hxx"
#include "PropertyMap.hxx"
#include "PropertyDescriptor.hxx"
#include "IntProperties.hxx"

#include <util/assert.hxx>
#include <logging/logging.hxx>

namespace DBricks {

Menu BoxShape::s_menu;

void
BoxShape::initialize_class()
{
    s_menu
        .append(new MenuItem("Aspect", "Fixed Aspect", new ShapeMenuAction<BoxShape, BoxShape::MenuActionMethodType>(&BoxShape::set_fixed_aspect)))
        .append(new MenuItem("Square", "Square",       new ShapeMenuAction<BoxShape, BoxShape::MenuActionMethodType>(&BoxShape::set_square)));
}

BoxShape::BoxShape(const Point& start, Handle*& handle)
    :Shape(start.x, start.y), m_x(m_corner.x), m_y(m_corner.y), m_width(40), m_height(40)
{
    handle = &m_sehandle;
    initialize();
}

BoxShape::BoxShape(const Rect& rect)
    :Shape(rect.x1(), rect.y1()), m_x(m_corner.x), m_y(m_corner.y),
     m_width(rect.width()), m_height(rect.height())
{
    initialize();
}

BoxShape::BoxShape(double x, double y, double width, double height)
    :Shape(x, y), m_x(m_corner.x), m_y(m_corner.y),
     m_width(width), m_height(height)
{
    initialize();
}


BoxShape::BoxShape()
    :m_x(m_corner.x), m_y(m_corner.y)
{
}

void
BoxShape::update_handles()
{
    m_lconnector.point(Point(m_x            , m_y + m_height/2));
    m_rconnector.point(Point(m_x + m_width  , m_y + m_height/2));
    m_tconnector.point(Point(m_x + m_width/2, m_y));
    m_bconnector.point(Point(m_x + m_width/2, m_y + m_height));

    m_nwconnector.point(Point(m_x,           m_y));
    m_neconnector.point(Point(m_x + m_width, m_y));
    m_seconnector.point(Point(m_x + m_width, m_y + m_height));
    m_swconnector.point(Point(m_x,           m_y + m_height));    

    
    m_lhandle.point(Point(m_x            , m_y + m_height/2));
    m_rhandle.point(Point(m_x + m_width  , m_y + m_height/2));
    m_thandle.point(Point(m_x + m_width/2, m_y));
    m_bhandle.point(Point(m_x + m_width/2, m_y + m_height));

    m_nwhandle.point(Point(m_x,           m_y));
    m_nehandle.point(Point(m_x + m_width, m_y));
    m_sehandle.point(Point(m_x + m_width, m_y + m_height));
    m_swhandle.point(Point(m_x,           m_y + m_height));
}

void
BoxShape::initialize()
{
    m_lconnector.initialize(this, Point(m_x            , m_y + m_height/2));
    m_rconnector.initialize(this, Point(m_x + m_width  , m_y + m_height/2));
    m_tconnector.initialize(this, Point(m_x + m_width/2, m_y));
    m_bconnector.initialize(this, Point(m_x + m_width/2, m_y + m_height));

    m_nwconnector.initialize(this, Point(m_x,           m_y));
    m_neconnector.initialize(this, Point(m_x + m_width, m_y));
    m_seconnector.initialize(this, Point(m_x + m_width, m_y + m_height));
    m_swconnector.initialize(this, Point(m_x,           m_y + m_height));

    
    m_lhandle.initialize(this, &m_lconnector, Point(m_x            , m_y + m_height/2));
    m_rhandle.initialize(this, &m_rconnector, Point(m_x + m_width  , m_y + m_height/2));
    m_thandle.initialize(this, &m_tconnector, Point(m_x + m_width/2, m_y));
    m_bhandle.initialize(this, &m_bconnector, Point(m_x + m_width/2, m_y + m_height));

    m_nwhandle.initialize(this, &m_nwconnector, Point(m_x,           m_y));
    m_nehandle.initialize(this, &m_neconnector, Point(m_x + m_width, m_y));
    m_sehandle.initialize(this, &m_seconnector, Point(m_x + m_width, m_y + m_height));
    m_swhandle.initialize(this, &m_swconnector, Point(m_x,           m_y + m_height));    


    m_handles.push_back(&m_thandle);
    m_handles.push_back(&m_nehandle);
    m_handles.push_back(&m_rhandle);
    m_handles.push_back(&m_sehandle);
    m_handles.push_back(&m_bhandle);
    m_handles.push_back(&m_swhandle);
    m_handles.push_back(&m_lhandle);
    m_handles.push_back(&m_nwhandle);
    
    m_connectors.push_back(&m_tconnector);
    m_connectors.push_back(&m_neconnector);
    m_connectors.push_back(&m_rconnector);
    m_connectors.push_back(&m_seconnector);
    m_connectors.push_back(&m_bconnector);
    m_connectors.push_back(&m_swconnector);
    m_connectors.push_back(&m_lconnector);
    m_connectors.push_back(&m_nwconnector);

    on_data_change();
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
    } else if (handle == &m_nwhandle) {
        m_x      += delta.x;
        m_width  -= delta.x;
        m_y      += delta.y;
        m_height -= delta.y;
    } else if (handle == &m_nehandle) {
        m_width  += delta.x;
        m_y      += delta.y;
        m_height -= delta.y;
    } else if (handle == &m_sehandle) {
        m_width  += delta.x;
        m_height += delta.y;        
    } else if (handle == &m_swhandle) {
        m_x      += delta.x;
        m_width  -= delta.x;
        m_height += delta.y;        
    } else {
        ASSERT_NOT_REACHED();
    }
    
    update_handles();

    on_data_change();
}

void
BoxShape::move_connector(Connector* connector, const Point& delta)
{
    if (connector == &m_lconnector) {
        m_x     += delta.x;
        m_width -= delta.x;
    } else if (connector == &m_rconnector) {
        m_width += delta.x;
    } else if (connector == &m_tconnector) {
        m_y      += delta.y;
        m_height -= delta.y;
    } else if (connector == &m_bconnector) {
        m_height += delta.y;
    } else if (connector == &m_nwconnector) {
        m_x      += delta.x;
        m_width  -= delta.x;
        m_y      += delta.y;
        m_height -= delta.y;
    } else if (connector == &m_neconnector) {
        m_x      -= delta.x;
        m_width  += delta.x;
        m_y      -= delta.y;
        m_height += delta.y;
    } else if (connector == &m_seconnector) {
        m_width  += delta.x;
        m_height += delta.y;        
    } else if (connector == &m_swconnector) {
        m_x      += delta.x;
        m_width  -= delta.x;
        m_y      += delta.y;
        m_height += delta.y;        
    } else {
        ASSERT_NOT_REACHED();
    }

    on_data_change();
}

void
BoxShape::move(const Point& delta)
{
    m_corner += delta;
    update_handles();
    on_data_change();
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

Menu*
BoxShape::menu(const Point& point) const
{
    return &s_menu;
}

void
BoxShape::set_fixed_aspect()
{
    DLOG(DIAGRAM, DEBUG, "set_fixed_aspect is called\n");
}

void
BoxShape::set_square()
{
    DLOG(DIAGRAM, DEBUG, "set_square is called\n");
}

void
BoxShape::property_apply()
{
    initialize();
}

static const char* property_layout =
    "(table :rows 2 :cols 2"
    "    (property \"X\" x)         (property \"Y\" y)"
    "    (property \"Width\" width) (property \"Height\" height)"
    ")";


Gtk::Widget*
BoxShape::property_widget()
{
    if (!m_property_descriptor) {
        m_property_map = new PropertyMap(this);

        m_property_map->add(new NumberProperty<double>("X", m_x, 0, 400));
        m_property_map->add(new NumberProperty<double>("Y", m_y, 0, 400));
        m_property_map->add(new NumberProperty<double>("WIDTH", m_width, 0, 200));
        m_property_map->add(new NumberProperty<double>("HEIGHT", m_height, 0, 200));

        m_property_descriptor = new PropertyDescriptor(*m_property_map, property_layout);
    }

    return m_property_descriptor->to_widget();
}

} // namespace DBricks
