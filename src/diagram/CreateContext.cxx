#include "CreateContext.hxx"

#include <logging/logging.hxx>

#include "defines.hxx"
#include "Diagram.hxx"
#include "ShapeFactory.hxx"
#include "gtkutil.hxx"
#include "Handle.hxx"
#include "Shape.hxx"
#include "BuiltinChanges.hxx"

#include "snap.hxx"

namespace DBricks {

bool
CreateContext::on_button_press_event(Shape* shape, GdkEventButton* e)
{
    if (e->button == Left_Button) {
        Point start = snap(point_of_event(e));
        
        m_state = CC_Dragging;
        m_shape = ShapeFactory::create_shape(m_shape_type, start, m_handle);
        m_shape->show_handles();
        m_diagram->add_shape(m_shape);
    }
    
    return false;
}

bool
CreateContext::on_motion_notify_event(Shape* shape, GdkEventMotion* e)
{
    if (m_state == CC_Dragging) {
        Point point = snap(point_of_event(e));
        Diagram::move_handle(m_shape, m_handle, point - m_handle->point());
    }

    return false;
}

bool
CreateContext::on_button_release_event(Shape* shape, GdkEventButton* e)
{
    if (m_state == CC_Dragging) {
        m_diagram->history().append(new ShapeCreateChange(m_diagram, m_shape));
        
        m_shape->hide_handles();
        m_shape  = NULL;
        m_handle = NULL;
        m_state  = CC_None;
    }

    return false;
}

} // namespace DBricks
