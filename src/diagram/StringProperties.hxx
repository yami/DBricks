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

    virtual Gtk::Widget* to_widget(const std::string& text)
    {
        m_entry = new Gtk::Entry(m_value);
        return m_entry;
    }

    virtual void on_apply()
    {
        if (m_entry)
            m_value = m_entry->get_text();
    }

    virtual bool is_labeled() const
    {
        return true;
    }

private:
    Gtk::Entry*  m_entry;
    std::string& m_value;
};


} // namespace DBricks


#endif // STRINGPROPERTIES_HXX
