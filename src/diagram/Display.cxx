#include "Display.hxx"

#include <algorithm>
#include <functional>

#include <cairomm/context.h>

#include "ModifyContext.hxx"
#include "Diagram.hxx"
#include "Shape.hxx"

namespace DBricks {

Display::Display(Diagram* diagram)
    :DiagramObserver(diagram), m_context(new ModifyContext(diagram))
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

bool
Display::on_event(GdkEvent* event)
{
    m_context->on_event(event);
    return true;
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
        //std::for_each(m_diagram->shapes().begin(), m_diagram->shapes().end(),
        //              std::mem_fun(&Shape::draw));

        window->end_paint();
    }
}


} // namespace DBricks
