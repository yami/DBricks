#ifndef EVENTCONTEXT_HXX
#define EVENTCONTEXT_HXX

#include <gdkmm/event.h>

namespace DBricks {

class Diagram;
class Display;

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
    
    virtual bool on_event(GdkEvent *event)
    {
    }
protected:
    Diagram* m_diagram;
    Display* m_display;
};


} // namespace DBricks

#endif // EVENTCONTEXT_HXX
