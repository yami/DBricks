#ifndef CREATECONTEXT_HXX
#define CREATECONTEXT_HXX

#include "DispatchContext.hxx"

namespace DBricks {

class CreateContext : public DispatchContext {
public:
    CreateContext(Diagram* diagram, Display* display)
        :DispatchContext(diagram, display)
    {
    }

private:
    virtual bool on_button_press_event(Shape* shape, GdkEventButton* e);
};


} // namespace DBricks

#endif // CREATECONTEXT_HXX
