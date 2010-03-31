#include "PropertyDescriptor.hxx"

#include "PropertyMap.hxx"
#include "Property.hxx"

#include <util/assert.hxx>
#include <ssexp/ssexp.h>

#include <gtkmm/table.h>
#include <gtkmm/label.h>

using namespace ssexp;

namespace {

struct LabeledWidget {
    LabeledWidget(const std::string& l, Gtk::Widget* w)
        :label(l), widget(w)
    {
    }

    LabeledWidget(Gtk::Widget* w)
        :label(), widget(w)
    {
    }
    
    std::string  label;
    Gtk::Widget* widget;
};

LabeledWidget sexp_evaluate_layout(sexp_t list, const DBricks::PropertyMap& property_map)
{
    sexp_t container = car(list);
    
    std::string container_name = ci_to_string(symbol_name(container));

    if (container_name == "TABLE") {
        sexp_t rest = cdr(list);

        sexp_t dim = car(rest); rest = cdr(rest);
        sexp_t val = car(rest); rest = cdr(rest);
        std::string dim_name  = ci_to_string(symbol_name(dim));
        int         val_value = ci_to_integer(val);

        int nrow, ncol;

        if (dim_name == ":ROWS") {
            nrow = val_value;
        } else if (dim_name == ":COLS") {
            ncol = val_value;
        }

        dim = car(rest); rest = cdr(rest);
        val = car(rest); rest = cdr(rest);

        dim_name  = ci_to_string(symbol_name(dim));
        val_value = ci_to_integer(val);

        if (dim_name == ":ROWS") {
            nrow = val_value;
        } else if (dim_name == ":COLS") {
            ncol = val_value;
        }

        Gtk::Table* table = new Gtk::Table(nrow*2, ncol*2);
        for (int irow = 0; irow < nrow; ++irow) {
            for (int icol = 0; icol < ncol; ++icol) {
                sexp_t item = car(rest); rest = cdr(rest);
                LabeledWidget labeledWidget = sexp_evaluate_layout(item, property_map);

                Gtk::Label*  label = new Gtk::Label(labeledWidget.label);
                Gtk::Widget* widget = labeledWidget.widget;

                table->set_col_spacing(2*icol+1, 50);
                
                if (labeledWidget.label != "") {
                    label->set_alignment(0.0, 0.5);
                    
                    table->attach(*label,              2*icol,   2*icol+1, irow, irow+1, Gtk::FILL, Gtk::FILL);
                    table->attach(*widget, 2*icol+1,   2*icol+2, irow, irow+1, Gtk::FILL|Gtk::EXPAND, Gtk::FILL);
                } else {
                    table->attach(*widget, 2*icol,     2*icol+2, irow, irow+1, Gtk::FILL|Gtk::EXPAND, Gtk::FILL);
                }
            }
        }

        return LabeledWidget(table);
    } else if (container_name == "PROPERTY") {
        std::string label = ci_to_string(second(list));
        std::string id    = ci_to_string(symbol_name(third(list)));

        DBricks::Property* property = property_map.get(id);

        if (property->is_labeled())
            return LabeledWidget(label, property->widget(label));
        else
            return LabeledWidget("",    property->widget(label));
    } else {
        ASSERT_NOT_REACHED();
    }
}

} // namespace anonymous

namespace DBricks {

void
PropertyDescriptor::on_ok()
{
    m_property_map.save();
    m_main.hide();
}

void
PropertyDescriptor::on_apply()
{
    m_property_map.save();
}

void
PropertyDescriptor::on_cancel()
{
    m_main.hide();
}


Gtk::Widget*
PropertyDescriptor::to_widget()
{
    if (!m_widgetized) {
        m_widgetized = true;
        
        Gtk::Widget* user_widget = evaluate_layout();
        m_main.add(m_vbox);

        m_vbox.pack_start(*user_widget);
        m_vbox.pack_start(m_hbox);

        m_hbox.pack_start(m_ok);
        m_hbox.pack_start(m_apply);
        m_hbox.pack_start(m_cancel);

        m_ok.signal_clicked().connect(sigc::mem_fun(*this, &PropertyDescriptor::on_ok));
        m_apply.signal_clicked().connect(sigc::mem_fun(*this, &PropertyDescriptor::on_apply));
        m_cancel.signal_clicked().connect(sigc::mem_fun(*this, &PropertyDescriptor::on_cancel));
    }

    m_property_map.load();
    
    return &m_main;
}

Gtk::Widget* PropertyDescriptor::evaluate_layout()
{
    sexp_t layout = read_from_string(m_property_layout.c_str());
    return sexp_evaluate_layout(layout, m_property_map).widget;
}


} // namespace DBricks
