#ifndef IWITHPROPERTIES_HXX
#define IWITHPROPERTIES_HXX

namespace Gtk {
class Widget;
};

namespace DBricks {

class IWithProperties {
public:
    virtual Gtk::Widget* property_widget() = 0;
    virtual void         property_apply() = 0;
};

} // namespace DBricks


#endif // IWITHPROPERTIES_HXX
