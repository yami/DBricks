#ifndef PROPERTYDESCRIPTOR_HXX
#define PROPERTYDESCRIPTOR_HXX

#include <gtkmm/window.h>
#include <gtkmm/hbox.h>
#include <gtkmm/vbox.h>
#include <gtkmm/button.h>

namespace DBricks {

class PropertyDescriptor {
public:
    PropertyDescriptor(const PropertyMap& property_map, const std::string& property_layout)
        :m_property_map(property_map), m_property_layout(property_layout), m_widgetized(false)
    {
    }

    Gtk::Widget* to_widget();
    
private:
    Gtk::Widget* evaluate_layout();
    
    const PropertyMap& m_property_map;
    const std::string& m_property_layout;

    bool m_widgetized;

    Gtk::Button m_ok;
    Gtk::Button m_cancel;
    Gtk::Button m_apply;

    Gtk::VBox m_vbox;
    Gtk::HBox m_hbox;

    Gtk::Window m_main;
};

} // namespace DBricks

#endif // PROPERTYDESCRIPTOR_HXX
