#ifndef PROPERTY_HXX
#define PROPERTY_HXX

#include <string>

namespace Gtk {
class Widget;
};

namespace DBricks {

struct Property {
    Property(const std::string& id)
        :m_id(id)
    {
    }
    
    virtual ~Property() {}
    virtual Gtk::Widget* widget(const std::string& text) = 0;
    virtual void         save_to_value()  = 0;
    virtual void         load_to_widget() = 0;
    virtual bool         is_labeled() const = 0;
    
    std::string id()
    {
        return m_id;
    }

    std::string m_id;
};

} // namespace DBricks

#endif // PROPERTY_HXX
