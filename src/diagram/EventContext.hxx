#ifndef EVENTCONTEXT_HXX
#define EVENTCONTEXT_HXX

#include <gdkmm/event.h>

namespace DBricks {

class Diagram;

class EventContext {
public:
    EventContext (Diagram* diagram)
        :m_diagram(diagram)
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
};


} // namespace DBricks

#endif // EVENTCONTEXT_HXX
