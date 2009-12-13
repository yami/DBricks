#include "Diagram.hxx"
#include "DiagramObserver.hxx"
#include "Shape.hxx"
#include "Handle.hxx"
#include "Connector.hxx"


#include <geom/Point.hxx>
#include <util/stl.hxx>

namespace DBricks {

void
Diagram::add_shape(Shape* shape)
{
    m_shapes.push_back(shape);
}

void
Diagram::del_shape(Shape* shape)
{
    util::delete_value(m_shapes, shape);
}

Shape*
Diagram::find_closest_shape(const Point& point) const
{
    /* @FIXME */
    double max_dist = 30.0;
    Shape* closest  = 0;
    double min_dist = 0;

    ShapesType::const_iterator iter = m_shapes.begin();
    for(; iter != m_shapes.end(); ++iter) {
        if ((*iter)->cover(point)) {
            closest = *iter;
            min_dist = closest->distance(point);
        }
    }

    if (!closest) 
        return 0;

    for(++iter; iter != m_shapes.end(); ++iter) {
        if ((*iter)->cover(point)) {
            double dist = (*iter)->distance(point);
            if (dist < min_dist) {
                closest  = *iter;
                min_dist = dist;
            }
        }
    }

    return closest;
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


Connector*
Diagram::find_closest_connector(const std::vector<Shape*>& shapes, const Point& point) const
{
    double min_dist = 3;
    Connector* closest = 0;
    
    for (ShapesType::const_iterator siter = m_shapes.begin();
         siter != m_shapes.end();
         ++siter) {
        if (util::in_container(shapes, *siter))
            continue;

        for (Shape::ConnectorsType::const_iterator citer = (*siter)->connectors().begin();
             citer != (*siter)->connectors().end();
             ++citer) {
            if ((*citer)->distance(point) < min_dist) {
                closest = *citer;
                min_dist = (*citer)->distance(point);
            }
        }
    }

    return closest;
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

void
Diagram::move_shapes(std::vector<Shape*>& shapes, const Point& delta)
{
    // We did not consider rotation right now, so every point (hence connector)
    // moves same delta.
    //
    // and we assume a all of a moved shape's handles are moved too.
    // 
    for (std::vector<Shape*>::iterator siter = shapes.begin();
         siter != shapes.end();
         ++siter) {
        (*siter)->move(delta);

        if ((*siter)->break_connections()) {
            std::for_each((*siter)->connectors().begin(), (*siter)->connectors().end(), Connector::break_connections);
        } else {
            for (Shape::ConnectorsType::iterator citer = (*siter)->connectors().begin();
                 citer != (*siter)->connectors().end();
                 ++citer) {
                move_connector(*citer, delta);
            }
        }
    }
}

void
Diagram::move_connector(Connector* connector, const Point& delta)
{
    Connector::ConnectorsType& connectors = connector->connectors();
    for (Connector::ConnectorsType::iterator iter = connectors.begin();
         iter != connectors.end();
         ++iter) {
        Shape* shape = (*iter)->shape();
        shape->move_connector(*iter, delta);
    }
}

void
Diagram::move_handle(Shape* shape, Handle* handle, const Point& delta)
{
    shape->move_handle(handle, delta);
    
    if (handle->connector()) {
        if (shape->break_connections()) {
            Connector::break_connections(handle->connector());
        } else {
            move_connector(handle->connector(), delta);
        }
    }
}

} // namespace DBricks
