#ifndef DESKTOP_HXX
#define DESKTOP_HXX

#include <gtkmm/window.h>
#include <gtkmm/ruler.h>
#include <gtkmm/table.h>
#include <gtkmm/scrollbar.h>
#include <gtkmm/box.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/menu.h>
#include <gtkmm/toggletoolbutton.h>
#include <gtkmm/toolpalette.h>

#include "Display.hxx"
#include "Diagram.hxx"
#include "Clipboard.hxx"

namespace DBricks {

class Desktop : public Gtk::Window {
public:
    Desktop();
    
    // Gtk::Windows interfaces
    virtual bool on_event(GdkEvent* event);

    // called by Display
    void on_display_event(GdkEvent* event);
private:
    void on_new_file();
    void on_open_file();
    void on_save_file();
    void on_quit_program();
    void on_create_shape(const std::string& shape_type);
    void on_select_modify();
    void on_select_zoom();
    void on_edit_copy();
    void on_edit_paste();
    void on_edit_undo();

    void on_palette_item_toggled(Gtk::ToggleToolButton* button, const std::string& shape_type);
    
    void initialize_menus();
    void initialize_layout();

    void init_palette();
    void init_drawing_space();
    
    Gtk::Widget* create_menubar();
    
    Diagram        m_diagram;   // diagram must be constructed before display
    Display        m_display;
    ShapeClipboard m_clipbard;
    
    Glib::RefPtr<Gtk::ActionGroup> m_action_group;
    Glib::RefPtr<Gtk::UIManager>   m_ui_manager;
    
    Gtk::VBox        m_vbox;
    Gtk::HBox        m_hbox;
    Gtk::ToolPalette m_palette;
    Gtk::Table       m_table;
    Gtk::HRuler      m_hruler;
    Gtk::VRuler      m_vruler;
    Gtk::HScrollbar  m_hscrollbar;
    Gtk::VScrollbar  m_vscrollbar;
};

} // namespace DBricks

#endif // DESKTOP_HXX
