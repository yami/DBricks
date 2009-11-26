#include "ModifyContext.hxx"

#include <logging/logging.hxx>
#include <util/assert.hxx>

#include "defines.hxx"
#include "Shape.hxx"
#include "Handle.hxx"
#include "Diagram.hxx"
#include "Display.hxx"
#include "GroupShape.hxx"

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
    Point point(e->x, e->y);
    
    if (e->button == Left_Button) {
        pick_current_shape(shape);

        if (!m_selected_shapes.empty()) {
            m_selected_handle = m_diagram->find_closest_handle(m_selected_shapes[0], point);
            if (m_selected_handle) {
                bit_set(m_state, HandleMoving);
            } else {
                bit_set(m_state, Dragging);
                m_display->set_cursor(Gdk::FLEUR);
            }
        } else {
            bit_set(m_state, Selecting);
        }
        
        m_mpoint.x = e->x;
        m_mpoint.y = e->y;
        m_opoint   = m_mpoint;
    } else if (e->button == Right_Button) {
        if (!m_selected_shapes.empty()) {
            DLOG(DIAGRAM, DEBUG, "grouping...");

            GroupShape* group = new GroupShape(m_selected_shapes.begin(), m_selected_shapes.end());
            m_diagram->add_shape(group);
            for (std::vector<Shape*>::iterator iter = m_selected_shapes.begin();
                 iter != m_selected_shapes.end();
                 ++iter) {
                m_diagram->del_shape(*iter);
            }

            std::for_each(m_selected_shapes.begin(), m_selected_shapes.end(), std::mem_fun(&Shape::hide_handles));
            m_selected_shapes.clear();
            m_selected_shapes.push_back(group);
            std::for_each(m_selected_shapes.begin(), m_selected_shapes.end(), std::mem_fun(&Shape::show_handles));
        }
    }

    return pass_down;
}

bool
ModifyContext::on_motion_notify_event(Shape* shape, GdkEventMotion* e)
{
    bool pass_down = false;
        
    Point point(e->x, e->y);

    if (bit_is_set(m_state, Dragging)) {
        m_display->set_cursor(Gdk::FLEUR);
        for (std::vector<Shape*>::iterator iter = m_selected_shapes.begin();
             iter != m_selected_shapes.end();
             ++iter) {
            (*iter)->move(point - m_mpoint);
        }
    } else if (bit_is_set(m_state, Selecting)) {
        m_display->selecting(m_opoint, m_mpoint, point);
    } else if (bit_is_set(m_state, HandleMoving)) {
        m_display->set_cursor(Gdk::X_CURSOR);
        m_selected_shapes[0]->move_handle(m_selected_handle, point - m_mpoint);
    } else {
        if (!m_selected_shapes.empty() && m_diagram->find_closest_handle(m_selected_shapes[0], point)) {
            m_display->set_cursor(Gdk::X_CURSOR);
        } else {
            m_display->set_cursor();
        }
    }

    m_mpoint = point;    

    return pass_down;
}

bool
ModifyContext::on_button_release_event(Shape* shape, GdkEventButton* e)
{
    Point point(e->x, e->y);

    if (bit_is_set(m_state, Selecting)) {
        m_display->selected(m_opoint, point);

        std::for_each(m_selected_shapes.begin(), m_selected_shapes.end(), std::mem_fun(&Shape::hide_handles));
        m_selected_shapes.clear();
        
        for(Diagram::ShapesType::const_iterator iter = m_diagram->shapes().begin();
            iter != m_diagram->shapes().end();
            ++iter) {
            if ((*iter)->in(Rect(m_opoint, point))) {
                m_selected_shapes.push_back(*iter);
            }
        }

        std::for_each(m_selected_shapes.begin(), m_selected_shapes.end(), std::mem_fun(&Shape::show_handles));        
    }

    bit_zero(m_state);
    m_display->set_cursor();
    
    return true;
}

}
