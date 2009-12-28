#ifndef EVENTCONTEXT_HXX
#define EVENTCONTEXT_HXX

#include <gdkmm/event.h>
#include <geom/Point.hxx>

namespace DBricks {

class Diagram;
class Display;
class Shape;
class Menu;

class EventContext {
public:
    EventContext (Diagram* diagram, Display* display)
        :m_diagram(diagram), m_display(display)
    {
    }
    
    virtual ~EventContext ()
    {
    }
    
    virtual void on_activate()
    {
    }
    
    virtual void on_deactivate()
    {
    }
    
    virtual bool on_event(Shape* shape, GdkEvent *event)
    {
    }

    virtual Menu* menu(const Point& point) const
    {
        return 0;
    }
protected:
    Diagram* m_diagram;
    Display* m_display;
};


} // namespace DBricks

#endif // EVENTCONTEXT_HXX
