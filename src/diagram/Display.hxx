#ifndef DISPLAY_HXX
#define DISPLAY_HXX

#include <gtkmm/window.h>

#include "DiagramObserver.hxx"


namespace DBricks {

class Diagram;
class EventContext;

class Display : public DiagramObserver, public Gtk::Window {
public:
    Display(Diagram* diagram);
    ~Display();

    virtual void update();
    virtual bool on_event(GdkEvent* event);
    
private:
    void draw(GdkEventExpose* event=NULL);
    
    EventContext* m_context;
};

} // namespace DBricks

#endif // DISPLAY_HXX
