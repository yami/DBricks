#ifndef MODIFYCONTEXT_HXX
#define MODIFYCONTEXT_HXX

#include <vector>

#include "DispatchContext.hxx"
#include "Menu.hxx"
#include <geom/Point.hxx>

namespace DBricks {

class Shape;
class Handle;
class Connector;

class ModifyContext : public DispatchContext {
public:
    ModifyContext(Diagram* diagram, Display* display)
        :DispatchContext(diagram, display), m_selected_handle(0)
    {
    }

    static void initialize_class();
    
    virtual Menu* menu(const Point& point) const
    {
        return &s_menu;
    }

    virtual void reset();
private:
    typedef void (ModifyContext::*MenuActionMethodType) ();
    
    void group_shapes();
    void stack_forward();
    void stack_backward();
    
    virtual bool on_button_press_event(Shape* shape, GdkEventButton* e);
    virtual bool on_button_release_event(Shape* shape, GdkEventButton* e);
    virtual bool on_motion_notify_event(Shape* shape, GdkEventMotion* e);

    bool pick_current_shape(Shape* shape, const Point& point);
    Connector* find_closest_connector(const std::vector<Shape*>& shapes, const Point& point) const;
    
    static const unsigned int Dragging     = 0x1;
    static const unsigned int Selecting    = 0x2;
    static const unsigned int HandleMoving = 0x4;

    static Menu s_menu;
    
    Point        m_mpoint;
    Point        m_opoint;
    unsigned int m_state;
    Handle*      m_selected_handle;
};

}
#endif // MODIFYCONTEXT_HXX
