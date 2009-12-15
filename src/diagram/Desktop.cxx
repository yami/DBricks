#include "Desktop.hxx"

#include <logging/logging.hxx>

namespace DBricks {

Desktop::Desktop()
    :m_display(&m_diagram), m_table(3,3)
{
    m_diagram.attach_observer(&m_display);

    init_layout();
    
    add_events(Gdk::BUTTON_PRESS_MASK);
    add_events(Gdk::POINTER_MOTION_MASK);
    add_events(Gdk::EXPOSURE_MASK);
}

bool
Desktop::on_event(GdkEvent* event)
{
    if (event->type == GDK_MOTION_NOTIFY) {
        GdkEventMotion* e = reinterpret_cast<GdkEventMotion*>(event);
        m_hruler.property_position().set_value(e->x);
        m_vruler.property_position().set_value(e->y);
    }
    
    return false;
}

void
Desktop::init_layout()
{
    set_default_size(400, 500);

    add(m_table);
    
    m_hruler.set_range(0.0, 400.0, 0.0, 400.0);
    m_vruler.set_range(0.0, 500.0, 0.0, 500.0);

    // 0,0
    // 1,0    
    m_table.attach(m_hruler, 1, 2, 0, 1, Gtk::EXPAND|Gtk::SHRINK|Gtk::FILL, Gtk::FILL);
    // 2,0

    // 0,1
    m_table.attach(m_vruler, 0, 1, 1, 2, Gtk::FILL, Gtk::EXPAND|Gtk::SHRINK|Gtk::FILL);
    // 1,1
    m_table.attach(m_display, 1, 2, 1, 2, Gtk::EXPAND|Gtk::SHRINK|Gtk::FILL, Gtk::EXPAND|Gtk::SHRINK|Gtk::FILL);
    // 2,1
    m_table.attach(m_vscrollbar, 2, 3, 1, 2, Gtk::FILL, Gtk::EXPAND|Gtk::SHRINK|Gtk::FILL);

    // 0,2
    // 1,2
    m_table.attach(m_hscrollbar, 1, 2, 2, 3, Gtk::EXPAND|Gtk::SHRINK|Gtk::FILL, Gtk::FILL);
    // 2,2

    show_all_children();
}

} // namespace DBricks
