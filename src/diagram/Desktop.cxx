#include "Desktop.hxx"

#include <logging/logging.hxx>

#include <gtkmm/stock.h>

#include <fstream>

#include "SML.hxx"
#include "ecl.hxx"

#include "gtkmm/filechooserdialog.h"

namespace DBricks {

Desktop::Desktop()
    :m_display(&m_diagram), m_vbox(), m_table(3,3),
     m_action_group(Gtk::ActionGroup::create()), m_ui_manager(Gtk::UIManager::create())
{
    m_diagram.attach_observer(&m_display);

    initialize_menus();
    initialize_layout();
    
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
Desktop::on_new_file()
{
    DLOG(DIAGRAM, DEBUG, "on new file...\n");
}

void
Desktop::on_save_file()
{
    DLOG(DIAGRAM, DEBUG, "on save file...\n");

    Gtk::FileChooserDialog dialog("Save to a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

    switch (dialog.run()) {
        case Gtk::RESPONSE_OK:
        {
            Glib::ustring filename = dialog.get_filename().c_str();
            DLOG(DIAGRAM, DEBUG, "save file with name = %s\n", filename.c_str());
            
            Sml::Object* diagram_object = new Sml::Object();
            m_diagram.save(diagram_object);

            std::ofstream ofile(filename.c_str());
            EclSml::EclSerializer s(ofile);
            s.save_object(diagram_object);    
        }
        default:
            DLOG(DIAGRAM, DEBUG, "save to a file exited...\n");
    }
}

void
Desktop::on_open_file()
{
    DLOG(DIAGRAM, DEBUG, "on open file\n");

    Gtk::FileChooserDialog dialog("Open a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

    switch (dialog.run()) {
        case Gtk::RESPONSE_OK:
        {
            Glib::ustring filename = dialog.get_filename().c_str();
            DLOG(DIAGRAM, DEBUG, "open file with name = %s\n", filename.c_str());
            m_diagram.reset();
            m_display.reset();
            
            Sml::Object* diagram_object = EclSml::load(filename.c_str());
            m_diagram.load(diagram_object);
                
            break;
        }
        default:
            DLOG(DIAGRAM, DEBUG, "open file exited...\n");
    }
}

void
Desktop::on_quit_program()
{
    DLOG(DIAGRAM, DEBUG, "on quit...\n");
}

void
Desktop::initialize_menus()
{
    m_action_group->add(
        Gtk::Action::create("FileMenu", "File"));
    m_action_group->add(
        Gtk::Action::create("FileNewStandard", Gtk::Stock::NEW, "_New", "Create a new file"),
        sigc::mem_fun(*this, &Desktop::on_new_file));
    m_action_group->add(
        Gtk::Action::create("FileOpenStandard", Gtk::Stock::OPEN, "_Open", "Open"),
        sigc::mem_fun(*this, &Desktop::on_open_file));
    m_action_group->add(
        Gtk::Action::create("FileSaveStandard", Gtk::Stock::SAVE, "_Save", "Save"),
        sigc::mem_fun(*this, &Desktop::on_save_file));
    m_action_group->add(
        Gtk::Action::create("FileQuitStandard", Gtk::Stock::QUIT, "_Quit", "Quit the program"),
        sigc::mem_fun(*this, &Desktop::on_quit_program));

    m_ui_manager->insert_action_group(m_action_group);
    Glib::ustring ui_info =
        "<ui>"
        "  <menubar>"
        "    <menu action='FileMenu'>"
        "      <menuitem action='FileOpenStandard' />"        
        "      <menuitem action='FileNewStandard' />"
        "      <menuitem action='FileSaveStandard' />"
        "      <menuitem action='FileQuitStandard' />"
        "    </menu>"
        "  </menubar>"
        "</ui>";
    m_ui_manager->add_ui_from_string(ui_info);
}

Gtk::Widget*
Desktop::create_menubar()
{
    return m_ui_manager->get_widget("/menubar");
}

void
Desktop::initialize_layout()
{
    set_default_size(400, 500);

    // add main vbox
    add(m_vbox);

    // add menubar to main vbox
    Gtk::Widget* menubar = create_menubar();
    m_vbox.pack_start(*menubar, Gtk::PACK_SHRINK);

    // add table to main vbox
    m_vbox.pack_start(m_table, Gtk::PACK_EXPAND_WIDGET);
    
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
