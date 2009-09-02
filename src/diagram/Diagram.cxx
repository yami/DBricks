#include "Diagram.hxx"
#include "DiagramObserver.hxx"
#include "Shape.hxx"
#include "Handle.hxx"

#include <geom/Point.hxx>
#include <util/stl.hxx>

namespace DBricks {

void
Diagram::add_shape(Shape* shape)
{
    m_shapes.push_back(shape);
}

Shape*
Diagram::find_closest_shape(const Point& point) const
{
    /* @FIXME */
    double max_dist = 300.0;

    for (ShapesType::const_iterator iter = m_shapes.begin();
         iter != m_shapes.end();
         ++iter) {
        if ((*iter)->distance(point) <= max_dist) {
            return *iter;
        }
    }

    return 0;
}

Handle*
Diagram::find_closest_handle(Shape* shape, const Point& point) const
{
    if (!shape)
        return 0;
        
    /** @FIXME */
    double max_dist = 3.0;
    Shape::HandlesType& handles = shape->handles();

    Point delta_point = point;
    delta_point.x -= shape->corner().x;
    delta_point.y -= shape->corner().y;
        
    for (Shape::HandlesType::const_iterator iter = handles.begin();
         iter != handles.end();
         ++iter) {
        if ((*iter)->distance(delta_point) <= max_dist)
            return *iter;
    }

    return 0;
}


const Diagram::ShapesType&
Diagram::shapes() const
{
    return m_shapes;
}

void
Diagram::notify_observers() const
{
    for (ObserversType::const_iterator iter=m_observers.begin();
         iter != m_observers.end();
         ++iter) {
        (*iter)->update();
    }
}

void
Diagram::attach_observer(DiagramObserver* observer)
{
    m_observers.push_back(observer);
}

void
Diagram::detach_observer(DiagramObserver* observer)
{
    util::delete_value(m_observers, observer);
}

} // namespace DBricks
