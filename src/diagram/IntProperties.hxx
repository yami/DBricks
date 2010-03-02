#ifndef INTPROPERTIES_HXX
#define INTPROPERTIES_HXX

#include "Property.hxx"
#include <gtkmm/adjustment.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/combobox.h>
#include <gtkmm/treemodel.h>
//#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/liststore.h>

namespace DBricks {

template<class NumberT>
class NumberProperty : public Property {
public:
    NumberProperty(const std::string& id, NumberT& value, float lower, float upper, float step = 1)
        :Property(id), m_value(value), m_adjustment(m_value, lower, upper, step), m_spin_button(m_adjustment, 1.0, 0)
    {
    }

    virtual Gtk::Widget* widget(const std::string& text)
    {
        return &m_spin_button;
    }

    virtual void save_to_value()
    {
        m_value = static_cast<NumberT>(m_spin_button.get_value());
    }

    virtual void load_to_widget()
    {
        m_spin_button.set_value(m_value);
    }

    virtual bool is_labeled() const
    {
        return true;
    }

private:
    NumberT& m_value;
    
    Gtk::Adjustment m_adjustment;
    Gtk::SpinButton m_spin_button;
};


class BoolProperty : public Property {
public:
    BoolProperty(const std::string& id, bool& value)
        :Property(id), m_value(value), m_check_button(0)
    {
    }

    ~BoolProperty()
    {
        delete m_check_button;
    }
    
    virtual Gtk::Widget* widget(const std::string& text)
    {
        if (!m_check_button)
            m_check_button =  new Gtk::CheckButton(text);
        
        return m_check_button;
    }

    virtual void save_to_value()
    {
        if (m_check_button)
            m_value = m_check_button->get_active();
    }

    virtual void load_to_widget()
    {
        if (m_check_button) {
            m_check_button->set_active(m_value);
        }
    }
    
    virtual bool is_labeled() const
    {
        return false;
    }

private:
    bool&             m_value;
    Gtk::CheckButton* m_check_button;
};



template<class EnumT>
class EnumProperty : public Property {
private:
    template<class T>
    struct ModelColumns : public Gtk::TreeModel::ColumnRecord {
        ModelColumns()
        {
            add(m_enum_value);
            add(m_enum_string);
        }

        Gtk::TreeModelColumn<T> m_enum_value;
        Gtk::TreeModelColumn<std::string> m_enum_string;
    };

public:
    EnumProperty(const std::string& id, EnumT& value,
                 const std::vector<EnumT>& enum_values, const std::vector<std::string>& enum_strings);
    
    virtual Gtk::Widget* widget(const std::string& text)
    {
        return &m_combo_box;
    }

    virtual void save_to_value()
    {
        Gtk::TreeModel::iterator iter = m_combo_box.get_active();

        if (iter) {
            Gtk::TreeModel::Row row = *iter;                
            m_value = row[m_columns.m_enum_value];
        }
    }

    virtual void load_to_widget()
    {
        m_combo_box.set_active(m_value);
    }
    
    virtual bool is_labeled() const
    {
        return true;
    }

private:
    EnumT&                   m_value;
    
    Gtk::ComboBox                m_combo_box;
    Glib::RefPtr<Gtk::ListStore> m_model;
    ModelColumns<EnumT>          m_columns;
};


template<class EnumT>
EnumProperty<EnumT>::EnumProperty(const std::string& id, EnumT& value,
                                  const std::vector<EnumT>& enum_values, const std::vector<std::string>& enum_strings)
    :Property(id), m_value(value)
{
    assert(enum_values.size() == enum_strings.size());
    
    m_model     = Gtk::ListStore::create(m_columns);
    m_combo_box.set_model(m_model);

    for (size_t i = 0; i< enum_values.size(); ++i) {
        Gtk::TreeModel::Row row = *(m_model->append());
        row[m_columns.m_enum_value]  = enum_values[i];
        row[m_columns.m_enum_string] = enum_strings[i];
    }

    m_combo_box.pack_start(m_columns.m_enum_string);
    m_combo_box.set_active(0);
}


} // namespace DBricks

#endif // INTPROPERTIES_HXX
