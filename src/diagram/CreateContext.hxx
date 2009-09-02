#ifndef CREATECONTEXT_HXX
#define CREATECONTEXT_HXX

#include "DispatchContext.hxx"

namespace DBricks {

class CreateContext : public DispatchContext {
public:
    CreateContext(Diagram* diagram)
        :DispatchContext(diagram)
    {
    }

private:
    virtual bool on_button_press_event(GdkEventButton* e);
};


} // namespace DBricks

#endif // CREATECONTEXT_HXX
