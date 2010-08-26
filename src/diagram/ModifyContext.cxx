#include "ModifyContext.hxx"

#include <logging/logging.hxx>
#include <util/assert.hxx>

#include "defines.hxx"
#include "Shape.hxx"
#include "Handle.hxx"
#include "Diagram.hxx"
#include "Display.hxx"
#include "GroupShape.hxx"
#include "Menu.hxx"
#include "BuiltinChanges.hxx"
#include "snap.hxx"


#include <util/stl.hxx>
#include <util/bit.hxx>

#include <algorithm>
#include <functional>

namespace DBricks {

Menu ModifyContext::s_menu;

void
ModifyContext::initialize_class()
{
    s_menu
        .append(new MenuItem("Group",        "Group",           new ShapeMenuAction<ModifyContext, ModifyContext::MenuActionMethodType>(&ModifyContext::group_shapes)))
        .append(new MenuItem("StackForward", "Stack Forward",   new ShapeMenuAction<ModifyContext, ModifyContext::MenuActionMethodType>(&ModifyContext::stack_forward)))
        .append(new MenuItem("StackBackward", "Stack Backward", new ShapeMenuAction<ModifyContext, ModifyContext::MenuActionMethodType>(&ModifyContext::stack_backward)))
        .append(new MenuItem("Properties",    "Properties",     new ShapeMenuAction<ModifyContext, ModifyContext::MenuActionMethodType>(&ModifyContext::show_property)));
}

// TODO: add near-selection
// near-selection:
//   when there's already a shape selected, and new click point is outside of this selected shape
//   normal the old shape will be unselected. However if the new click point is closest to
//   a handle of the old selected shape, we should select the old shape. Otherwise it is hard
//   to grab a handle...
bool
ModifyContext::pick_current_shape(Shape* shape, const Point& point)
{
    const bool changed = true;

    Selection& selection = m_diagram->selection();
    if (selection.is_selected(shape)) {
        return !changed;
    }
    
    if (shape) {
        selection.unselect();
        selection.select(shape);
    } else {
        // near-selection works for single selection only. below is non-near-selection condidtion
        if (selection.size() != 1 || !m_diagram->find_closest_handle(selection[0], point)) {
            selection.unselect();
        }
    }
    
    return changed;
}

bool
ModifyContext::on_button_press_event(Shape* shape, GdkEventButton* e)
{
    bool pass_down = false;
    Point point(e->x, e->y);

    Selection& selection = m_diagram->selection();
    
    if (e->button == Left_Button) {
        pick_current_shape(shape, point);

        if (!selection.empty()) {
            m_selected_handle = selection.size() > 1 ? NULL : m_diagram->find_closest_handle(selection[0], point);

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
        if (!selection.empty()) {
            m_display->popup(this, selection[0], e);
        }
    }

    return pass_down;
}

bool
ModifyContext::on_motion_notify_event(Shape* shape, GdkEventMotion* e)
{
    bool pass_down = false;
    Point point(e->x, e->y);

    Selection& selection = m_diagram->selection();
    
    if (bit_is_set(m_state, Dragging)) {
        m_display->set_cursor(Gdk::FLEUR);
        Diagram::move_shapes(selection.shapes(), point - m_mpoint);
    } else if (bit_is_set(m_state, Selecting)) {
        m_display->selecting(m_opoint, m_mpoint, point);
    } else if (bit_is_set(m_state, HandleMoving)) {
        if (m_selected_handle->connector() && m_selected_handle->connector()->is_active()) {
            m_display->set_cursor(Gdk::BLANK_CURSOR);

            m_display->set_highlight_closest_connector();

        } else {
            m_display->set_cursor(Gdk::X_CURSOR);
        }
        Diagram::move_handle(selection[0], m_selected_handle, point - m_mpoint);
    } else {
        if (selection.size() == 1 && m_diagram->find_closest_handle(selection[0], point)) {
            m_display->set_cursor(Gdk::X_CURSOR);
        } else {
            m_display->set_cursor();
        }
    }

    m_mpoint = point;    

    return pass_down;
}

// TODO: move to another file
template<class ShapeContainerT>
Rect calc_bounding_box(const ShapeContainerT& shapes)
{
    typename ShapeContainerT::const_iterator iter = shapes.begin();
    Rect rect((*iter)->bb());

    for (++iter; iter != shapes.end(); ++iter) {
        Rect curr((*iter)->bb());

        rect = Rect(std::min(rect.x1(), curr.x1()),
                    std::min(rect.y1(), curr.y1()),
                    std::max(rect.x2(), curr.x2()),
                    std::max(rect.y2(), curr.y2()));
    }

    return rect;
}

bool
ModifyContext::on_button_release_event(Shape* shape, GdkEventButton* e)
{
    Point point(e->x, e->y);

    Selection& selection = m_diagram->selection();
    History&   history   = m_diagram->history();
    
    if (bit_is_set(m_state, Dragging)) {
        Rect  bb = calc_bounding_box(selection.shapes());
        Point source = Point(bb.x1(), bb.y1());
        Point target = snap(source);
        
        Diagram::move_shapes(selection.shapes(),  target - source + point - m_mpoint);
        history.append(new ShapeMoveChange(m_diagram, selection.shapes(), m_opoint, point + target - source));
    } else if (bit_is_set(m_state, Selecting)) {
        m_display->selected(m_opoint, point);

        selection.unselect();
        selection.select(Rect(m_opoint, point), m_diagram->shapes().begin(), m_diagram->shapes().end());
    } else if (bit_is_set(m_state, HandleMoving)) {
        assert(selection.size() == 1);
        Connector* connector1 = this->find_closest_connector(selection.shapes(), point);
        Connector* connector2 = m_diagram->find_closest_connector(selection.shapes(), point);

        if (connector1 && connector2) {
            // TODO: FIXME. this is a dirty hack.
            Handle* h1 = connector1->shape()->handle(connector1->point());
            if (h1)
                h1->point(connector2->point());
            
            connector1->point(connector2->point());
            Connector::build_connections(connector1, connector2);
        }
        
        history.append(new HandleMoveChange(m_diagram, m_selected_handle, m_opoint, point));
    }

    bit_zero(m_state);
    m_display->set_cursor();
    
    return true;
}

Connector*
ModifyContext::find_closest_connector(const std::vector<Shape*>& shapes, const Point& point) const
{
    double min_dist = 10;
    Connector* closest = 0;
    
    if (shapes.empty())
        return closest;    
    
    for (std::vector<Shape*>::const_iterator iter = shapes.begin();
         iter != shapes.end();
         ++iter) {
        for (Shape::ConnectorsType::const_iterator c = (*iter)->connectors().begin();
             c != (*iter)->connectors().end();
             ++c) {
            if ((*c)->distance(point) < min_dist) {
                closest = *c;
                min_dist = (*c)->distance(point);
            }
        }
    }

    return closest;
}

void
ModifyContext::group_shapes()
{
    Selection& selection = m_diagram->selection();
    GroupShape* group = new GroupShape(selection.shapes().begin(), selection.shapes().end());
    m_diagram->add_shape(group);
    for (std::vector<Shape*>::iterator iter = selection.shapes().begin();
         iter != selection.shapes().end();
         ++iter) {
        m_diagram->del_shape(*iter);
    }
    
    selection.unselect();
    selection.select(group);
}

void
ModifyContext::stack_backward()
{
    Selection& selection = m_diagram->selection();
    
    if (selection.size() == 1)
        m_diagram->stack_backward(selection[0]);
}

void
ModifyContext::stack_forward()
{
    Selection& selection = m_diagram->selection();
    
    if (selection.size() == 1)
        m_diagram->stack_forward(selection[0]);
}

void
ModifyContext::show_property()
{
    Selection& selection = m_diagram->selection();
    
    Gtk::Widget* widget = selection[0]->property_widget();

    if (widget) {
        widget->show_all();
    }
}

void
ModifyContext::reset()
{
    m_selected_handle = 0;
    bit_zero(m_state);
    m_display->set_cursor();
}

}
