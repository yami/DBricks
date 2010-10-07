#include "Desktop.hxx"

#include <logging/logging.hxx>
#include <sml/Sml.hxx>

#include <gtkmm/stock.h>

#include <fstream>
#include <string>

#include "SexpSaver.hxx"
#include "SexpLoader.hxx"
#include "ShapeFactory.hxx"
#include "CreateContext.hxx"
#include "ModifyContext.hxx"
#include "ZoomContext.hxx"
#include "Clipboard.hxx"
#include "Change.hxx"
#include "DiagramArchiver.hxx"
#include "ZoomWindow.hxx"

#include "gtkmm/filechooserdialog.h"

namespace DBricks {

Desktop::Desktop()
    :m_display(&m_diagram, this), m_vbox(), m_table(3,3),
     m_action_group(Gtk::ActionGroup::create()), m_ui_manager(Gtk::UIManager::create()),
     m_hscroll_adjustment(0, 0, 400, 1, 100, 400),
     m_vscroll_adjustment(0, 0, 400, 1, 100, 400),
     m_hscrollbar(m_hscroll_adjustment),
     m_vscrollbar(m_vscroll_adjustment)
{
    m_diagram.attach_observer(&m_display);
    
    initialize_menus();
    initialize_layout();
    
    add_events(Gdk::BUTTON_PRESS_MASK);
    add_events(Gdk::POINTER_MOTION_MASK);
    add_events(Gdk::EXPOSURE_MASK);

    m_display.zwindow()->signal_changed().connect(sigc::mem_fun(this, &Desktop::on_zwindow));
}

bool
Desktop::on_event(GdkEvent* event)
{
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

    Gtk::FileChooserDialog dialog("Save to a file", Gtk::FILE_CHOOSER_ACTION_SAVE);
    dialog.set_transient_for(*this);
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

    switch (dialog.run()) {
        case Gtk::RESPONSE_OK:
        {
            Glib::ustring filename = dialog.get_filename().c_str();
            DLOG(DIAGRAM, DEBUG, "save file with name = %s\n", filename.c_str());

            DiagramArchiver ar;
            Sml::Object* diagram_object = new Sml::Object();
            ar.object(diagram_object);
            m_diagram.save(&ar);

            sml_to_sexp_file(filename.c_str(), diagram_object);
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
            
            Sml::Object* diagram_object = sexp_file_to_sml(filename.c_str());
            DiagramArchiver ar;
            ar.object(diagram_object);
            m_diagram.load(&ar);
                
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
Desktop::on_create_shape(const std::string& shape_type)
{
    m_display.set_context(new CreateContext(&m_diagram, &m_display, shape_type));
}

void
Desktop::on_select_modify()
{
    m_display.set_context(new ModifyContext(&m_diagram, &m_display));
}

void
Desktop::on_select_zoom()
{
    m_display.set_context(new ZoomContext(&m_diagram, &m_display));
}

void
Desktop::on_edit_copy()
{
    Selection& selection = m_diagram.selection();

    if (selection.empty()) {
        DLOG(DIAGRAM, DEBUG, "on_edit_copy: selection is empty!\n");
        return;
    }
    
    m_clipbard.put_clip(selection.shapes().begin(), selection.shapes().end());
}

void
Desktop::on_edit_paste()
{
    if (m_clipbard.empty()) {
        DLOG(DIAGRAM, DEBUG, "on_edit_paste: clipboard is empty!\n");
        return;
    }
    
    Selection& selection = m_diagram.selection();

    selection.unselect();
    ShapeList* shapes = m_clipbard.get_clip();

    for (ShapeList::iterator iter = shapes->begin(); iter != shapes->end(); ++iter)
        m_diagram.add_shape(*iter);
    
    selection.select(shapes->begin(), shapes->end());
    Diagram::move_shapes(*shapes, Point(10, 10));
    
    delete shapes;
    
    m_diagram.notify_observers();
}

void
Desktop::on_edit_undo()
{
    History& history = m_diagram.history();

    if (!history.empty()) {
        DLOG(DIAGRAM, DEBUG, "on edit undo: undo it\n");
    } else {
        DLOG(DIAGRAM, DEBUG, "on edit undo: history is empty\n");
        return;
    }
    
    Change* change = history.remove_change();
    change->revert();
    delete change;
    
    m_diagram.notify_observers();
}

void
Desktop::on_palette_item_toggled(Gtk::ToggleToolButton* button, const std::string& shape_type)
{
    if (button->get_active()) {
        m_display.set_context(new CreateContext(&m_diagram, &m_display, shape_type));
    } else {
        m_display.set_context(new ModifyContext(&m_diagram, &m_display));
    }
}

void
Desktop::on_zwindow(const ZoomWindow& zwindow)
{
    m_hruler.set_range(zwindow.x1(), zwindow.width(), zwindow.x1(), zwindow.width());
    m_vruler.set_range(zwindow.y1(), zwindow.height(), zwindow.y1(), zwindow.height());
}

void
Desktop::on_hscroll_changed()
{
    m_display.zoom(Point(m_hscroll_adjustment.get_value(), m_display.zwindow()->y1()),
                   1, Display::ZoomPoint_Origin);
}

void
Desktop::on_vscroll_changed()
{
    m_display.zoom(Point(m_display.zwindow()->x1(), m_vscroll_adjustment.get_value()),
                   1, Display::ZoomPoint_Origin);
}

extern ShapeTypeInventory theInventory;

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

    m_action_group->add(
        Gtk::Action::create("CreateMenu", "Create"));

    theInventory.action_group_init(m_action_group, sigc::mem_fun(*this, &Desktop::on_create_shape));
    
    // m_action_group->add(
    //     Gtk::Action::create("CreateRectangle", "Rectangle"),
    //     sigc::bind(sigc::mem_fun(*this, &Desktop::on_create_shape), "/standard/rectangle"));
    // m_action_group->add(
    //     Gtk::Action::create("CreateEllipse", "Ellipse"),
    //     sigc::bind(sigc::mem_fun(*this, &Desktop::on_create_shape), "/standard/ellipse"));
    // m_action_group->add(
    //     Gtk::Action::create("CreateLine", "Line"),
    //     sigc::bind(sigc::mem_fun(*this, &Desktop::on_create_shape), "/standard/line"));

    // m_action_group->add(
    //     Gtk::Action::create("CreatePredefinedProcess", "PredefinedProcess"),
    //     sigc::bind(sigc::mem_fun(*this, &Desktop::on_create_shape), "/boxed/predefined_process"));
    // m_action_group->add(
    //     Gtk::Action::create("CreateMerge", "Merge"),
    //     sigc::bind(sigc::mem_fun(*this, &Desktop::on_create_shape), "/boxed/merge"));

    
    m_action_group->add(
        Gtk::Action::create("ToolsMenu", "Tools"));
    m_action_group->add(
        Gtk::Action::create("ModifyTool", "Modify"),
        sigc::mem_fun(*this, &Desktop::on_select_modify));
    m_action_group->add(
        Gtk::Action::create("ZoomTool", "Zoom"),
        sigc::mem_fun(*this, &Desktop::on_select_zoom));


    m_action_group->add(
        Gtk::Action::create("EditMenu", "Edit"));
    m_action_group->add(
        Gtk::Action::create("EditCopy", "Copy"),
        sigc::mem_fun(*this, &Desktop::on_edit_copy));
    m_action_group->add(
        Gtk::Action::create("EditPaste", "Paste"),
        sigc::mem_fun(*this, &Desktop::on_edit_paste));
    m_action_group->add(
        Gtk::Action::create("EditUndo", "Undo"),
        sigc::mem_fun(*this, &Desktop::on_edit_undo));
    
    
    m_ui_manager->insert_action_group(m_action_group);
    Glib::ustring ui_info =
        "<ui>\n"
        "  <menubar>\n"
        "    <menu action='FileMenu'>\n"
        "      <menuitem action='FileOpenStandard' />\n"
        "      <menuitem action='FileNewStandard' />\n"
        "      <menuitem action='FileSaveStandard' />\n"
        "      <menuitem action='FileQuitStandard' />\n"
        "    </menu>\n"

        "    <menu action='ToolsMenu'>\n"
        "      <menuitem action='ModifyTool' />\n"
        "      <menuitem action='ZoomTool' />\n"
        "    </menu>\n"

        "    <menu action='EditMenu'>\n"
        "      <menuitem action='EditCopy' />\n"
        "      <menuitem action='EditPaste' />\n"
        "      <menuitem action='EditUndo' />\n"
        "    </menu>\n"
        
        "    <menu action='CreateMenu'>\n"
        + theInventory.ui_info(2, "  ") +
        "    </menu>\n"
        "  </menubar>\n"
        "</ui>\n";
/*
    "      <menuitem action='CreateRectangle' />"
        "      <menuitem action='CreateEllipse' />"
        "      <menuitem action='CreateLine' />"
        "      <menuitem action='CreatePredefinedProcess' />"
        "      <menuitem action='CreateMerge' />"        
*/   
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

    m_vbox.pack_start(m_hbox, Gtk::PACK_EXPAND_WIDGET);
    
    // add hbox for containing tool_palette and drawing_space
    init_drawing_space();
    init_palette();
    
    m_hbox.pack_start(m_palette, Gtk::PACK_SHRINK);
    m_hbox.pack_start(m_table, Gtk::PACK_EXPAND_WIDGET);
    
    show_all_children();
}

void
Desktop::init_palette()
{
    const ShapeTypeInventory::CollectionsType& collections = theInventory.collections();
    for (ShapeTypeInventory::CollectionsType::const_iterator citer = collections.begin();
         citer != collections.end();
         ++citer) {
        Gtk::ToolItemGroup* group = Gtk::manage(new Gtk::ToolItemGroup((*citer)->name()));
        m_palette.add(*group);
        
        for (ShapeTypeCollection::ShapeTypesType::const_iterator siter = (*citer)->shape_types().begin();
             siter != (*citer)->shape_types().end();
             ++siter) {
            Gtk::ToggleToolButton* button = Gtk::manage(new Gtk::ToggleToolButton((*siter)->short_name()));
            button->signal_toggled().connect(sigc::bind(sigc::mem_fun(*this, &Desktop::on_palette_item_toggled), button, (*siter)->name()));
            group->insert(*button);
        }
    }
}

void
Desktop::init_drawing_space()
{    
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

    m_hscroll_adjustment.signal_changed().connect(sigc::mem_fun(this, &Desktop::on_hscroll_changed));
    m_vscroll_adjustment.signal_changed().connect(sigc::mem_fun(this, &Desktop::on_vscroll_changed));
}

void
Desktop::on_display_event(GdkEvent* event)
{
    if (event->type == GDK_MOTION_NOTIFY) {
        // mouse position is changed.
        GdkEventMotion* e = reinterpret_cast<GdkEventMotion*>(event);
        m_hruler.property_position().set_value(e->x);
        m_vruler.property_position().set_value(e->y);
    }
}

} // namespace DBricks
