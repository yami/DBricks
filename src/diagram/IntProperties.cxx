#include "IntProperties.hxx"

namespace DBricks {

IntProperty::IntProperty(const std::string& id, int& value, float lower, float upper, float step = 1)
    :Property(id), m_value(value), m_adjustment(m_value, lower, upper, step), m_spin_button(m_adjustment, 1.0, 0)
{
}


Gtk::Widget*
IntProperty::to_widget(const std::string& text)
{
    return &m_spin_button;
}

void
IntProperty::on_apply()
{
    m_value = m_spin_button->get_value();
}


} // namespace DBricks
