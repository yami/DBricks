#ifndef MODIFYCONTEXT_HXX
#define MODIFYCONTEXT_HXX

#include "DispatchContext.hxx"
#include <geom/Point.hxx>

namespace DBricks {

class Shape;
class Handle;

class ModifyContext : public DispatchContext {
public:
    ModifyContext(Diagram* diagram, Display* display)
        :DispatchContext(diagram, display)
    {
    }
private:
    virtual bool on_button_press_event(GdkEventButton* e);

    virtual bool on_motion_notify_event(GdkEventMotion* e);
    
    enum State {
        None,
        Handle_Moving,
        Shape_Moving
    };

    Point   m_motion_opoint;
    State   m_state;
    Shape*  m_shape;
    Handle* m_handle;
};

}
#endif // MODIFYCONTEXT_HXX
