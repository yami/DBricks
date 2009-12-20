#include "Display.hxx"

#include <algorithm>
#include <functional>

#include <cairomm/context.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/menu.h>

#include <util/stl.hxx>
#include <logging/logging.hxx>

#include "defines.hxx"
#include "gtkutil.hxx"

#include "Shape.hxx"
#include "Handle.hxx"
#include "Diagram.hxx"
#include "Menu.hxx"


#include "ModifyContext.hxx"
#include "CreateContext.hxx"


namespace DBricks {

Display::Display(Diagram* diagram)
    :DiagramObserver(diagram), m_current_context(0), m_select_state(Select_None)
{
    add_events(Gdk::EXPOSURE_MASK);
    add_events(Gdk::POINTER_MOTION_MASK);
    add_events(Gdk::POINTER_MOTION_HINT_MASK);
    add_events(Gdk::BUTTON_PRESS_MASK);
    add_events(Gdk::BUTTON_RELEASE_MASK);
    add_events(Gdk::STRUCTURE_MASK);
    add_events(Gdk::ENTER_NOTIFY_MASK);
    add_events(Gdk::KEY_PRESS_MASK);
    add_events(Gdk::KEY_RELEASE_MASK);

    m_contexts.push_back(new ModifyContext(diagram, this));
    m_contexts.push_back(new CreateContext(diagram, this));
}


Display::~Display()
{
    util::delete_entries(m_contexts.begin(), m_contexts.end());
}

bool
Display::on_event(GdkEvent* event)
{
    if (event->type == GDK_BUTTON_PRESS) {
        GdkEventButton *e = (GdkEventButton*)event;

        if (e->button == Middle_Button) {
            m_current_context = (m_current_context + 1) % m_contexts.size();

            DLOG(DIAGRAM, INFO, "context changed!\n");
            return true;
        }
    }

    Point point;
    Shape* shape;
    
    if (point_of_event(event, &point)) {
        shape  = m_diagram->find_closest_shape(point);
    }
    
    m_contexts[m_current_context]->on_event(shape, event);

    update();
    return false;
}


void
Display::update()
{
    draw();
}


void
Display::draw(GdkEventExpose* event)
{
    Glib::RefPtr<Gdk::Window> window = get_window();
            
    if (window) {
        Gtk::Allocation allocation = get_allocation();

        const int width = allocation.get_width();
        const int height = allocation.get_height();

        // XXX: use my own double buffering?
        // double buffering

        Gdk::Rectangle rect(0, 0, width, height);
        window->begin_paint_rect(rect);
                
        Cairo::RefPtr<Cairo::Context> ctx = window->create_cairo_context();
                
        if (event) {
            ctx->rectangle(event->area.x, event->area.y,
                           event->area.width, event->area.height);
            ctx->clip();
        }

        ctx->set_source_rgba(1, 1, 1, 1);
        ctx->paint();
                
        ctx->set_source_rgba(0.337, 0.612, 0.117, 0.9);
        ctx->select_font_face("Sans",
                              Cairo::FONT_SLANT_NORMAL,
                              Cairo::FONT_WEIGHT_NORMAL);
        ctx->set_font_size(40.0);
        ctx->move_to(10.0, 50.0);
        ctx->show_text("Disziplin ist Macht.");

        for (Diagram::ShapesType::const_iterator iter=m_diagram->shapes().begin();
             iter != m_diagram->shapes().end();
             ++iter) {
            (*iter)->draw(ctx);
        }

        draw_select(ctx);

        window->end_paint();
    }
}

void
Display::draw_select(Cairo::RefPtr<Cairo::Context> ctx)
{
    switch (m_select_state) {
        case Select_None:
            break;
        case Select_Selecting:
        {
            std::vector<double> d;
            Rect rect(m_select_origin, m_select_current);
            d.push_back(4);
            d.push_back(1);
            
            ctx->save();
            ctx->rectangle(rect.x1(), rect.y1(), rect.width(), rect.height());
            ctx->set_dash(d, 0);
            ctx->stroke();
            ctx->restore();
            break;
        }
        case Select_Selected:
        {
            m_select_state = Select_None;
            break;
        }
    }
}

void
Display::set_cursor(Gdk::CursorType cursor_type)
{
    Glib::RefPtr<Gdk::Window> window = get_window();
    Gdk::Cursor cursor(cursor_type);
    window->set_cursor(cursor);
}

void
Display::set_cursor()
{
    get_window()->set_cursor();
}

Glib::ustring
menu_to_uiinfo(const Menu& menu)
{
    Glib::ustring uiinfo =
        "<ui>"
        "  <popup name='ShapeMenu'>";
    for (size_t i = 0; i<menu.size(); ++i) {
        uiinfo += "    <menuitem action='" + menu[i]->name() + "'/>";
    }

    uiinfo +=
        "  </popup>"
        "</ui>";

    return uiinfo;
}

Glib::RefPtr<Gtk::ActionGroup>
menu_to_action_group(Shape* shape, const Menu& menu)
{
    Glib::RefPtr<Gtk::ActionGroup> action_group = Gtk::ActionGroup::create();

    action_group->add(Gtk::Action::create("ShapeMenu", "Shape Menu"));
    for (size_t i = 0; i<menu.size(); ++i) {
        action_group->add(Gtk::Action::create(menu[i]->name().c_str(), menu[i]->text().c_str()),
                          sigc::bind(sigc::mem_fun(*menu[i], &MenuItem::operator()), shape));
    }

    return action_group;
}


void
Display::popup(Shape* shape, Menu* menu, GdkEventButton* event)
{
    Glib::RefPtr<Gtk::ActionGroup> action_group = menu_to_action_group(shape, *menu);
    Glib::ustring                  uiinfo       = menu_to_uiinfo(*menu);

    Glib::RefPtr<Gtk::UIManager> uimgr = Gtk::UIManager::create();
    uimgr->insert_action_group(action_group);
    uimgr->add_ui_from_string(uiinfo);

    Gtk::Menu* popup = dynamic_cast<Gtk::Menu*>(uimgr->get_widget("/ShapeMenu"));
    popup->popup(event->button, event->time);
}



} // namespace DBricks
