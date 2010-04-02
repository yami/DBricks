#include "Diagram.hxx"
#include "DiagramObserver.hxx"
#include "Shape.hxx"
#include "Handle.hxx"
#include "Connector.hxx"


#include <geom/Point.hxx>
#include <geom/computation.hxx>
#include <util/stl.hxx>
#include <algorithm>
#include <logging/logging.hxx>

#include "ShapeFactory.hxx"

#include <sml/Sml.hxx>

namespace DBricks {

void
Diagram::save(Sml::Object* object) const
{
    Sml::List* shape_list = new Sml::List();
    
    object->add_attribute_data("name"  , "Diagram");
    object->add_attribute_data("type"  , "Diagram");
    object->add_attribute_data("shapes", shape_list);
    
    for(ShapesType::const_iterator iter = m_shapes.begin();
        iter != m_shapes.end();
        ++iter) {
        Sml::Object* shape_object = new Sml::Object();
        shape_list->add_value(new Sml::Value(shape_object));
        (*iter)->save(shape_object);
    }
}


void
Diagram::load(Sml::Object* object)
{
    Sml::List* shape_list;
    object->get_attribute_data("shapes", shape_list);
    for(Sml::List::ValuesType::const_iterator iter = shape_list->values().begin();
        iter != shape_list->values().end();
        ++iter) {
        Sml::Object* shape_object  = (*iter)->get_object();
        std::string  shape_type    = shape_object->get_attribute_string("type");
        Shape* shape = ShapeFactory::create_shape(shape_type);
        shape->load(shape_object);
        add_shape(shape);
    }
}

void
Diagram::reset()
{
    util::delete_entries(m_shapes.begin(), m_shapes.end());
    m_shapes.clear();
}

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
    // if m_shapes is empty, then we have no begin().
    if (m_shapes.empty())
        return 0;

    // we should search from the back to the front, so that shape
    // close to stack_top is first found.    
    ShapesType::const_iterator iter = m_shapes.end();
    for(--iter; iter != m_shapes.begin(); --iter) {
        if ((*iter)->cover(point)) {
            return *iter;
        }
    }

    return (*iter)->cover(point) ? *iter : 0;    
}

Handle*
Diagram::find_closest_handle(Shape* shape, const Point& point) const
{
    if (!shape)
        return 0;
        
    /** @FIXME */
    double max_dist = 3.0;
    Shape::HandlesType& handles = shape->handles();

    for (Shape::HandlesType::const_iterator iter = handles.begin();
         iter != handles.end();
         ++iter) {
        if ((*iter)->distance(point) <= max_dist)
            return *iter;
    }

    return 0;
}


Connector*
Diagram::find_closest_connector(const std::vector<Shape*>& shapes, const Point& point) const
{
    double min_dist = 10;
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
Diagram::update_shape_connectiors(Shape* shape)
{
    std::vector<Shape*> moved_shapes;
    moved_shapes.push_back(shape);
    update_shape_connectiors_internal(shape, moved_shapes);
}

void
Diagram::update_shape_connectiors_internal(Shape* shape, std::vector<Shape*>& moved_shapes)
{
    const double Min_Move_Change = 0.001;
    
    for (Shape::ConnectorsType::iterator citer = shape->connectors().begin();
         citer != shape->connectors().end();
         ++citer) {
        Point delta = (*citer)->point() - (*citer)->last_point();
        if (point_origin_distance(delta) > Min_Move_Change) {
            (*citer)->reset_last_point();

            for (Connector::ConnectorsType::iterator iter = (*citer)->connectors().begin();
                 iter != (*citer)->connectors().end();
                 ++iter) {
                Shape* connected_shape = (*iter)->shape();
                if (connected_shape && !util::in_container(moved_shapes, connected_shape)) {
                    connected_shape->move_connector(*iter, delta);
                    moved_shapes.push_back(connected_shape);
                    update_shape_connectiors_internal(connected_shape, moved_shapes);
                }
            }
        }
    }
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

        if ((*siter)->break_connections()) 
            std::for_each((*siter)->connectors().begin(), (*siter)->connectors().end(), Connector::break_connections);
        else
            update_shape_connectiors(*siter);
    }
}


void
Diagram::move_handle(Shape* shape, Handle* handle, const Point& delta)
{
    shape->move_handle(handle, delta);

    if (handle->connector() && shape->break_connections())
            Connector::break_connections(handle->connector());
    
    update_shape_connectiors(shape);
}

void
Diagram::stack_forward(Shape* shape)
{
    if (m_shapes.size() < 2)
        return;

    ShapesType::iterator curr = std::find(m_shapes.begin(), m_shapes.end(), shape);
    ASSERT(curr != m_shapes.end());

    ShapesType::iterator next = curr; ++next;
    if (next == m_shapes.end())
        return;

    std::swap(*next, *curr);
}

void
Diagram::stack_backward(Shape* shape)
{
    if (m_shapes.size() < 2)
        return;

    ShapesType::iterator curr = std::find(m_shapes.begin(), m_shapes.end(), shape);
    ASSERT(curr != m_shapes.end());

    if (curr == m_shapes.begin())
        return;

    ShapesType::iterator prev = curr; --prev;
    std::swap(*prev, *curr);    
}

} // namespace DBricks
