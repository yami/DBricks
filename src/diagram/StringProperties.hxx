#ifndef STRINGPROPERTIES_HXX
#define STRINGPROPERTIES_HXX

#include "Property.hxx"

#include <gtkmm/entry.h>

namespace DBricks {

class StringProperty : public Property {
public:
    StringProperty(const std::string& id, std::string& value)
        :Property(id), m_value(value)
    {
    }

    virtual Gtk::Widget* widget(const std::string& text)
    {
        return &m_entry;
    }

    virtual void save_to_value()
    {
        m_value = m_entry->get_text();
    }

    virtual void load_to_widget()
    {
        m_entry.set_text(m_value);
    }

    virtual bool is_labeled() const
    {
        return true;
    }

private:
    Gtk::Entry   m_entry;
    std::string& m_value;
};


} // namespace DBricks


#endif // STRINGPROPERTIES_HXX
