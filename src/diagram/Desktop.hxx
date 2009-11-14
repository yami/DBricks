#ifndef DESKTOP_HXX
#define DESKTOP_HXX

#include <gtkmm/window.h>
#include <gtkmm/ruler.h>
#include <gtkmm/table.h>
#include <gtkmm/scrollbar.h>

#include "Display.hxx"
#include "Diagram.hxx"


namespace DBricks {

class Desktop : public Gtk::Window {
public:
    Desktop();
    
    // Gtk::Windows interfaces
    virtual bool on_event(GdkEvent* event);
private:
    void init_layout();
    
    Diagram m_diagram;          // diagram must be constructed before display
    Display m_display;

    Gtk::Table m_table;
    Gtk::HRuler m_hruler;
    Gtk::VRuler m_vruler;
    Gtk::HScrollbar m_hscrollbar;
    Gtk::VScrollbar m_vscrollbar;
};

} // namespace DBricks

#endif // DESKTOP_HXX
