#include "PropertyDescriptor.hxx"
#include "EclCommon.hxx"

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

static ecl_evaluate_layout(cl_object list, const PropertyMap& property_map)
{
    LabeledWidget process(cl_object list)
    {
        cl_object container = XCAR(list);
    
        std::string container_name = to_stl_string(cl_symbol_name(container));

        if (container_name == "TABLE") {
            cl_object rest = XCDR(list);

            cl_object dim = XCAR(rest); rest = XCDR(rest);
            cl_object val = XCAR(rest); rest = XCDR(rest);
            std::string dim_name = to_stl_string(cl_symbol_name(dim));
            int         val_value = fixint(val);

            int nrow, ncol;

            assert(ecl_keywordp(dim));
        
            if (dim_name == "ROWS") {
                nrow = val_value;
            } else if (dim_name == "COLS") {
                ncol = val_value;
            }

            dim = XCAR(rest); rest = XCDR(rest);
            val = XCAR(rest); rest = XCDR(rest);
            dim_name = to_stl_string(cl_symbol_name(dim));
            val_value = fixint(val);

            assert(ecl_keywordp(dim));
        
            if (dim_name == "ROWS") {
                nrow = val_value;
            } else if (dim_name == "COLS") {
                ncol = val_value;
            }

            Gtk::Table* table = new Gtk::Table(nrow*2, ncol*2);
            for (int irow = 0; irow < nrow; ++irow) {
                for (int icol = 0; icol < ncol; ++icol) {
                    cl_object item = XCAR(rest); rest = XCDR(rest);
                    LabeledWidget labeledWidget = process(item);

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
            std::string label = to_stl_string(XSECOND(list));
            std::string id    = to_stl_string(cl_symbol_name(XTHIRD(list)));

            Property* property = propertyMap.get(id);

            if (property->is_labeled())
                return LabeledWidget(label, property->to_widget(label));
            else
                return LabeledWidget("",    property->to_widget(label));
        } else {
            return LabeledWidget(0);
        }
    }
}

} // namespace anonymous

namespace DBricks {

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

        // TODO: connect signal handles.
    }

    return &m_main;
}

Gtk::Widget* PropertyDescriptor::evaluate_layout()
{
    cl_object layout = evaluate(m_property_layout);
    return ecl_evaluate_layout(layout).widget;
}


} // namespace DBricks
