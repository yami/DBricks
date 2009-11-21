#include "ModifyContext.hxx"

#include <logging/logging.hxx>
#include <util/assert.hxx>

#include "defines.hxx"
#include "Shape.hxx"
#include "Handle.hxx"
#include "Diagram.hxx"
#include "Display.hxx"

#include <util/stl.hxx>
#include <util/bit.hxx>

#include <algorithm>
#include <functional>

namespace DBricks {

bool
ModifyContext::pick_current_shape(Shape* shape)
{
    const bool changed = true;

    if (util::in_container(m_selected_shapes, shape)) {
        return !changed;
    }

    std::for_each(m_selected_shapes.begin(), m_selected_shapes.end(), std::mem_fun(&Shape::hide_handles));
    m_selected_shapes.clear();
    if (shape) {
        m_selected_shapes.push_back(shape);
    }
    std::for_each(m_selected_shapes.begin(), m_selected_shapes.end(), std::mem_fun(&Shape::show_handles));
    return changed;
}

bool
ModifyContext::on_button_press_event(Shape* shape, GdkEventButton* e)
{
    bool pass_down = false;

    if (e->button == Left_Button) {
        pick_current_shape(shape);

        if (!m_selected_shapes.empty())
            bit_set(m_state, Dragging);
        else
            bit_set(m_state, Selecting);

        m_mpoint.x = e->x;
        m_mpoint.y = e->y;
        m_opoint   = m_mpoint;
    }
    
    // if (e->button == Left_Button) {
    //     DLOG(DIAGRAM, DEBUG, "ModifyContext left button pressed \n");

    //     if (m_state == None) {
    //         m_shape  = m_display->event_shape();
    //         m_handle = m_display->event_handle();

    //         DLOG(DIAGRAM, DEBUG, "closest shape=%p, handle=%p\n", m_shape, m_handle);
            
    //         if (m_handle)
    //             m_state = Handle_Moving;
    //         else if (m_shape)
    //             m_state = Shape_Moving;
    //     } else {
    //         DLOG(DIAGRAM, DEBUG, "ModifyContext state is %d\n", (int)m_state);
    //         m_state     = None;
    //         m_shape     = 0;
    //         m_handle    = 0;
    //     }
    // }

    return pass_down;
}

bool
ModifyContext::on_motion_notify_event(Shape* shape, GdkEventMotion* e)
{
    bool pass_down = false;
        
    Point point(e->x, e->y);

    if (bit_is_set(m_state, Dragging)) {
        m_selected_shapes[0]->move(point - m_mpoint);
    } else if (bit_is_set(m_state, Selecting)) {
        m_display->selecting(m_opoint, m_mpoint, point);
    } else {
        DLOG(DIAGRAM, DEBUG, "on_motion_notify_event: dangling in else\n");
    }

    m_mpoint = point;
    
    // switch (m_state) {
    //     case Shape_Moving:
    //         ASSERT(m_shape);
    //         m_shape->move(delta);
    //         break;
    //     case Handle_Moving:
    //         ASSERT(m_shape);
    //         ASSERT(m_handle);
    //         m_shape->move_handle(m_handle, delta);
    //         break;
    //     case None:
    //         // do nothing
    //         break;
    // }

    return pass_down;
}

bool
ModifyContext::on_button_release_event(Shape* shape, GdkEventButton* e)
{
    Point point(e->x, e->y);
    
    if (bit_is_set(m_state, Dragging)) {
        bit_clr(m_state, Dragging);
    } else if (bit_is_set(m_state, Selecting)) {
        bit_clr(m_state, Selecting);
        m_display->selected(m_opoint, point);

        for(Diagram::ShapesType::const_iterator iter = m_diagram->shapes().begin();
            iter != m_diagram->shapes().end();
            ++iter) {
            if ((*iter)->in(Rect(m_opoint, point))) {
                std::for_each(m_selected_shapes.begin(), m_selected_shapes.end(), std::mem_fun(&Shape::hide_handles));
                m_selected_shapes.clear();
                m_selected_shapes.push_back(*iter);
                std::for_each(m_selected_shapes.begin(), m_selected_shapes.end(), std::mem_fun(&Shape::show_handles));
                break;          // FIXME: remove me when supporting selecting multiple shapes
            }
        }
    } else {
        DLOG(DIAGRAM, DEBUG, "on_button_release_event: dangling in else\n");
    }

    return true;
}

}
