#ifndef DISPLAY_HXX
#define DISPLAY_HXX

#include <vector>

#include <gtkmm/drawingarea.h>

#include "DiagramObserver.hxx"

// Perhaps I should change Display to Canvas or some other name?
// Its role is:
//   1. main drawing area
//   2. to receive events from user
//   3. to pass events to items in drawing area
//   4. to pass events to its parent

namespace DBricks {

class Diagram;
class EventContext;

class Display : public Gtk::DrawingArea, public DiagramObserver {
public:
    Display(Diagram* diagram);
    ~Display();

    // DBricks::DiagramObserver interfaces
    virtual void update();

    // Gtk::Window interfaces
    virtual bool on_event(GdkEvent* event);
private:
    void draw(GdkEventExpose* event=NULL);
    
    std::vector<EventContext*> m_contexts;
    size_t                     m_current_context;
};

} // namespace DBricks

#endif // DISPLAY_HXX
