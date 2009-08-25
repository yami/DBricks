#include "ModifyContext.hxx"
#include "Shape.hxx"
#include "Handle.hxx"
#include "Diagram.hxx"

#include <util/assert.hxx>
#include "defines.hxx"

namespace DBricks {

bool
ModifyContext::on_button_press_event(GdkEventButton* e)
{
    bool pass_down = false;

    if (e->button == Left_Button) {
        Point point(e->x, e->y);

        if (m_state = None) {
            m_shape  = m_diagram->find_closest_shape(point);
            m_handle = m_diagram->find_closest_handle(m_shape, point);

            if (m_handle)
                m_state = Handle_Moving;
            else if (m_shape)
                m_state = Shape_Moving;
        } else {
            m_state     = None;
            m_shape     = 0;
            m_handle    = 0;
        }
    }

    return pass_down;
}

bool
ModifyContext::on_motion_notify_event(GdkEventMotion* e)
{
    bool pass_down = false;
        
    Point point(e->x, e->y);
    Point delta = point - m_motion_opoint;

    m_motion_opoint = point;
                
    switch (m_state) {
        case Shape_Moving:
            ASSERT(m_shape);
            m_shape->move(delta);
            break;
        case Handle_Moving:
            ASSERT(m_shape);
            ASSERT(m_handle);
            m_shape->move_handle(m_handle, delta);
            break;
        case None:
            // do nothing
            break;
    }

    return pass_down;
}

}
