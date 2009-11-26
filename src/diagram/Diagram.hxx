#ifndef DIAGRAM_HXX
#define DIAGRAM_HXX

#include <list>

namespace DBricks {

class Shape;
class DiagramObserver;
class Handle;
class Point;

class Diagram
{
public:
    typedef std::list<Shape*> ShapesType;
    typedef std::list<DiagramObserver*> ObserversType;

    void add_shape(Shape* shape);
    void del_shape(Shape* shape);
    
    Shape* find_closest_shape(const Point& point) const;
    
    Handle* find_closest_handle(Shape* shape, const Point& point) const;    
    
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
