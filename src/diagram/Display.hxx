#ifndef DISPLAY_HXX
#define DISPLAY_HXX

#include <vector>

#include <gtkmm/window.h>

#include "DiagramObserver.hxx"


namespace DBricks {

class Diagram;
class EventContext;

class Display : public Gtk::Window, public DiagramObserver {
public:
    Display(Diagram* diagram);
    ~Display();

    virtual void update();
    virtual bool on_event(GdkEvent* event);
private:
    void draw(GdkEventExpose* event=NULL);
    
    std::vector<EventContext*> m_contexts;
    size_t                     m_current_context;
};

} // namespace DBricks

#endif // DISPLAY_HXX
