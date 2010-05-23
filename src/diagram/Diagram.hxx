#ifndef DIAGRAM_HXX
#define DIAGRAM_HXX

#include <list>
#include <vector>

#include "Selection.hxx"
#include "History.hxx"


namespace Sml {
class Object;
}

namespace DBricks {

class Shape;
class DiagramObserver;
class Handle;
class Point;
class Connector;
class Archiver;

class Diagram
{
public:
    typedef std::list<Shape*> ShapesType;
    typedef std::list<DiagramObserver*> ObserversType;

    static void move_shapes(std::vector<Shape*>& shapes, const Point& delta);
    static void move_handle(Shape* shape, Handle* handle, const Point& delta);
    static void update_shape_connectiors(Shape* shape);
    static void update_shape_connectiors_internal(Shape* shape, std::vector<Shape*>& moved_shapes);

    void reset();
    
    void save (Sml::Object* object) const;
    void load (Sml::Object* object);    

    void stack_forward(Shape* shape);
    void stack_backward(Shape* shape);
    
    
    void add_shape(Shape* shape);
    void del_shape(Shape* shape);
    
    Shape* find_closest_shape(const Point& point) const;
    
    Handle* find_closest_handle(Shape* shape, const Point& point) const;    

    Connector* find_closest_connector(const std::vector<Shape*>& shapes, const Point& point) const;
    
    const ShapesType& shapes() const;
    
    void notify_observers() const;

    void attach_observer(DiagramObserver* observer);

    void detach_observer(DiagramObserver* observer);

    Selection& selection()
    {
        return m_selection;
    }

    History& history()
    {
        return m_history;
    }
    
    struct NoopAction {
        void operator () (Connector* c)
        {
        }
    };

    template<class CloseActionT, class RemoteActionT>
    Connector* iterate_noselected_connectors(const std::vector<Shape*>& shapes, const Point& point,
                                             CloseActionT close_action, RemoteActionT remote_action) const;

private:
    
    ShapesType    m_shapes;
    ObserversType m_observers;
    Selection     m_selection;
    History       m_history;
};

template<class CloseActionT, class RemoteActionT>
Connector* Diagram::iterate_noselected_connectors(const std::vector<Shape*>& shapes, const Point& point,
                                                  CloseActionT close_action, RemoteActionT remote_action) const
{
    const double near_dist = 15;
    double       min_dist  = near_dist;
    Connector*   closest   = 0;

    std::vector<Connector*> near_connectors;
    near_connectors.reserve(5);
    
    for (ShapesType::const_iterator siter = m_shapes.begin();
         siter != m_shapes.end();
         ++siter) {
        if (util::in_container(shapes, *siter))
            continue;

        for (Shape::ConnectorsType::const_iterator citer = (*siter)->connectors().begin();
             citer != (*siter)->connectors().end();
             ++citer) {
            double dist = (*citer)->distance(point);

            if (dist < near_dist) {
                near_connectors.push_back(*citer);
            } else {
                remote_action(*citer);
            }
            
            if (dist < min_dist) {
                closest = *citer;
                min_dist = (*citer)->distance(point);
            }
        }
    }

    for (std::vector<Connector*>::iterator niter = near_connectors.begin();
         niter != near_connectors.end();
         ++niter) {
        if (*niter != closest)
            remote_action(*niter);
    }

    close_action(closest);
    
    return closest;
}

} // namespace DBricks

#endif // DIAGRAM_HXX
