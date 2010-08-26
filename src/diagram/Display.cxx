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
#include "Desktop.hxx"

#include "ModifyContext.hxx"
#include "CreateContext.hxx"


#include "CairoRenderer.hxx"

#include "snap.hxx"

namespace DBricks {

Display::Display(Diagram* diagram, Desktop* desktop)
    :DiagramObserver(diagram), m_desktop(desktop), m_context(new ModifyContext(diagram, this)), m_select_state(Select_None),
     m_renderer(0), m_highlight_closest_connector(false), m_event(NULL)
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
}


Display::~Display()
{
    delete m_context;
}

void
Display::set_context(EventContext* context)
{
    delete m_context;
    m_context = context;
}

void
Display::reset()
{
    m_context->reset();
}

bool
Display::on_event(GdkEvent* event)
{
    Point point;
    Shape* shape;

    m_event = event;
    
    if (point_of_event(event, &point)) {
        shape  = m_diagram->find_closest_shape(point);
    }
    
    m_context->on_event(shape, event);
    m_desktop->on_display_event(event);
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
        // better way?
        if (!m_renderer)
            m_renderer = new CairoRenderer(get_window());
            //m_renderer = new CairoRenderer(get_window()->create_cairo_context());
        
        
        Gtk::Allocation allocation = get_allocation();

        const int width = allocation.get_width();
        const int height = allocation.get_height();

        Gdk::Rectangle rect(0, 0, width, height);
        window->begin_paint_rect(rect);
                
        // if (event) {
        //     ctx->rectangle(event->area.x, event->area.y,
        //                    event->area.width, event->area.height);
        //     ctx->clip();
        // }

        m_renderer->begin_render(Rect(0, 0, width, height));
        m_renderer->draw_background(Rect(0, 0, width, height));
        
        draw_grid(width, height);


        Connector* closest = 0;
        
        if (m_highlight_closest_connector && m_event) {
            Point point;

            if (point_of_event(m_event, &point)) {
                Selection& selection = m_diagram->selection();
                closest = m_diagram->find_closest_connector(selection.shapes(), point);

                if (closest)
                    closest->highlight(true);
            }
            m_highlight_closest_connector = false;
        }
        
        
        for (Diagram::ShapesType::const_iterator iter=m_diagram->shapes().begin();
             iter != m_diagram->shapes().end();
             ++iter) {
            (*iter)->draw(m_renderer);
        }

        if (closest)
            closest->highlight(false);
        
        draw_select();
        
        
        m_renderer->end_render();
        
        window->end_paint();
    }
}

void
Display::draw_grid(int width, int height)
{
    const int xstep = 40;
    const int ystep = 40;

    snap_set_grid(xstep, ystep);
    

    m_renderer->save();

    //m_renderer->line_color(Color(0.337, 0.612, 0.117));
    m_renderer->line_width(0.2);
    
    for (int x=xstep; x < width; x += xstep) {
        m_renderer->draw_line(Point(x, 0), Point(x, height));
    }

    for (int y=ystep; y < height; y += ystep) {
        m_renderer->draw_line(Point(0, y), Point(width, y));
    }

    m_renderer->restore();
}

void
Display::draw_select()
{
    switch (m_select_state) {
        case Select_None:
            break;
        case Select_Selecting:
        {
            Rect rect(m_select_origin, m_select_current);

            m_renderer->save();
            m_renderer->line_style(LS_Dash);
            m_renderer->draw_rectangle(Point(rect.x1(), rect.y1()), Point(rect.x2(), rect.y2()), Draw_Stroke);
            m_renderer->restore();
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
menu_to_uiinfo(const Menu* pcontext_menu, const Menu* pshape_menu)
{
    Glib::ustring uiinfo =
        "<ui>"
        "  <popup name='ShapeMenu'>";

    if (pshape_menu) {
        const Menu& shape_menu = *pshape_menu;
        for (size_t i = 0; i<shape_menu.size(); ++i) {
            uiinfo += "    <menuitem action='" + shape_menu[i]->name() + "'/>";
        }
    }

    if (pcontext_menu) {
        const Menu& context_menu = *pcontext_menu;
        for (size_t i = 0; i<context_menu.size(); ++i) {
            uiinfo += "    <menuitem action='" + context_menu[i]->name() + "'/>";
        }
    }

    uiinfo +=
        "  </popup>"
        "</ui>";

    return uiinfo;
}

Glib::RefPtr<Gtk::ActionGroup>
menu_to_action_group(EventContext* context, const Menu* pcontext_menu, Shape* shape, const Menu* pshape_menu)
{
    Glib::RefPtr<Gtk::ActionGroup> action_group = Gtk::ActionGroup::create();

    action_group->add(Gtk::Action::create("ShapeMenu", "Shape Menu"));

    if (pshape_menu) {
        const Menu& shape_menu = *pshape_menu;
        for (size_t i = 0; i<shape_menu.size(); ++i) {
            action_group->add(Gtk::Action::create(shape_menu[i]->name().c_str(), shape_menu[i]->text().c_str()),
                              sigc::bind(sigc::mem_fun(*shape_menu[i], &MenuItem::operator()), (void*)shape));
        }
    }

    if (pcontext_menu) {
        const Menu& context_menu = *pcontext_menu;
        for (size_t i = 0; i<context_menu.size(); ++i) {
            action_group->add(Gtk::Action::create(context_menu[i]->name().c_str(), context_menu[i]->text().c_str()),
                              sigc::bind(sigc::mem_fun(*context_menu[i], &MenuItem::operator()), (void*)context));
        }
    }
    
    return action_group;
}


void
Display::popup(EventContext* context, Shape* shape, GdkEventButton* event)
{
    Menu* shape_menu   = shape->menu(Point(event->x, event->y));
    Menu* context_menu = context->menu(Point(event->x, event->y));
    
    Glib::RefPtr<Gtk::ActionGroup> action_group = menu_to_action_group(context, context_menu, shape, shape_menu);
    Glib::ustring                  uiinfo       = menu_to_uiinfo(context_menu, shape_menu);

    Glib::RefPtr<Gtk::UIManager> uimgr = Gtk::UIManager::create();
    uimgr->insert_action_group(action_group);
    uimgr->add_ui_from_string(uiinfo);

    Gtk::Menu* popup = dynamic_cast<Gtk::Menu*>(uimgr->get_widget("/ShapeMenu"));
    popup->popup(event->button, event->time);
}



} // namespace DBricks
