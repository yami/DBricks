#ifndef DISPATCHCONTEXT_HXX
#define DISPATCHCONTEXT_HXX

#include "EventContext.hxx"

namespace DBricks {

class DispatchContext : public EventContext {
public:
    DispatchContext(Diagram* diagram, Display* display)
        :EventContext(diagram, display)
    {
    }
    
    virtual bool on_event(Shape* shape, GdkEvent *event);
private:
    virtual bool on_button_press_event(Shape* shape, GdkEventButton* e)
    {
        return true;
    }

    virtual bool on_motion_notify_event(Shape* shape, GdkEventMotion* e)
    {
        return true;
    }

    virtual bool on_button_release_event(Shape* shape, GdkEventButton*e )
    {
        return true;
    }
    
    Shape* m_shape;
};

} // namespace DBricks

#endif // DISPATCHCONTEXT_HXX
