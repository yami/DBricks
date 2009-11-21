#ifndef MODIFYCONTEXT_HXX
#define MODIFYCONTEXT_HXX

#include <vector>

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
    virtual bool on_button_press_event(Shape* shape, GdkEventButton* e);
    virtual bool on_button_release_event(Shape* shape, GdkEventButton* e);
    virtual bool on_motion_notify_event(Shape* shape, GdkEventMotion* e);

    bool pick_current_shape(Shape* shape);
    
    static const unsigned int Dragging  = 0x1;
    static const unsigned int Selecting = 0x2;

    Point   m_mpoint;
    Point   m_opoint;
    Shape*  m_shape;
    Handle* m_handle;
    unsigned int   m_state;

    std::vector<Shape*> m_selected_shapes;
};

}
#endif // MODIFYCONTEXT_HXX
