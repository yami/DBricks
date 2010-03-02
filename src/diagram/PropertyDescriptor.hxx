#ifndef PROPERTYDESCRIPTOR_HXX
#define PROPERTYDESCRIPTOR_HXX

#include <string>

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/stock.h>

#include "PropertyMap.hxx"
#include "Property.hxx"

namespace DBricks {

class PropertyDescriptor {
public:
    PropertyDescriptor(const PropertyMap& property_map, const std::string& property_layout)
        :m_property_map(property_map), m_property_layout(property_layout), m_widgetized(false),
         m_ok(Gtk::Stock::OK), m_apply(Gtk::Stock::APPLY), m_cancel(Gtk::Stock::CANCEL)
    {
    }

    Gtk::Widget* to_widget();
    
private:
    void on_ok();
    void on_apply();
    void on_cancel();
    
    Gtk::Widget* evaluate_layout();
    
    const PropertyMap& m_property_map;
    const std::string& m_property_layout;

    bool m_widgetized;

    Gtk::Button m_ok;
    Gtk::Button m_apply;
    Gtk::Button m_cancel;

    Gtk::VBox m_vbox;
    Gtk::HBox m_hbox;

    Gtk::Window m_main;
};

} // namespace DBricks

#endif // PROPERTYDESCRIPTOR_HXX
