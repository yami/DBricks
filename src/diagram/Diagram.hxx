#ifndef DIAGRAM_HXX
#define DIAGRAM_HXX

#include <list>
#include <vector>

#include "Serializable.hxx"

namespace DBricks {

class Shape;
class DiagramObserver;
class Handle;
class Point;
class Connector;
class Archiver;

class Diagram : public Serializable
{
public:
    typedef std::list<Shape*> ShapesType;
    typedef std::list<DiagramObserver*> ObserversType;

    static void move_shapes(std::vector<Shape*>& shapes, const Point& delta);    
    static void move_handle(Shape* shape, Handle* handle, const Point& delta);
    static void update_shape_connectiors(Shape* shape);
    static void update_shape_connectiors_internal(Shape* shape, std::vector<Shape*>& moved_shapes);

    virtual void serialize(Archiver* ar) const;

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
private:
    ShapesType m_shapes;
    ObserversType m_observers;
};

} // namespace DBricks

#endif // DIAGRAM_HXX
