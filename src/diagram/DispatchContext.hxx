#ifndef DISPATCHCONTEXT_HXX
#define DISPATCHCONTEXT_HXX

#include "EventContext.hxx"

namespace DBricks {

class DispatchContext : public EventContext {
public:
    DispatchContext(Diagram* diagram)
        :EventContext(diagram)
    {
    }
    
    virtual bool on_event(GdkEvent *event);
private:
    virtual bool on_button_press_event(GdkEventButton* e)
    {
        return true;
    }

    virtual bool on_motion_notify_event(GdkEventMotion* e)
    {
        return true;
    }
};

} // namespace DBricks

#endif // DISPATCHCONTEXT_HXX
