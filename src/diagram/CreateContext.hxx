#ifndef CREATECONTEXT_HXX
#define CREATECONTEXT_HXX

#include "DispatchContext.hxx"
#include <string>


namespace DBricks {

class Handle;

class CreateContext : public DispatchContext {
public:
    CreateContext(Diagram* diagram, Display* display, const std::string& shape_type)
        :DispatchContext(diagram, display), m_shape_type(shape_type), m_handle(0)
    {
    }

private:
    enum State {
        CC_None,
        CC_Dragging
    };
    virtual bool on_button_press_event(Shape* shape, GdkEventButton* e);
    virtual bool on_motion_notify_event(Shape* shape, GdkEventMotion* e);
    virtual bool on_button_release_event(Shape* shape, GdkEventButton* e);

    std::string m_shape_type;
    Shape*      m_shape;
    Handle*     m_handle;
    State       m_state;
};


} // namespace DBricks

#endif // CREATECONTEXT_HXX
