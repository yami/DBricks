#ifndef DISPLAY_HXX
#define DISPLAY_HXX

#include <vector>

#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>
#include <gdkmm/cursor.h>

#include <geom/Point.hxx>
#include "DiagramObserver.hxx"

// Perhaps I should change Display to Canvas or some other name?
// Its role is:
//   1. main drawing area
//   2. to receive events from user
//   3. to pass events to items in drawing area
//   4. to pass events to its parent

namespace DBricks {

class Diagram;
class Shape;
class Handle;
class EventContext;
class Menu;

class Display : public Gtk::DrawingArea, public DiagramObserver {
public:
    Display(Diagram* diagram);
    ~Display();

    // DBricks::DiagramObserver interfaces
    virtual void update();

    // Gtk::Window interfaces
    virtual bool on_event(GdkEvent* event);

    // own interfaces
    void selecting(const Point& origin, const Point& last, const Point& current)
    {
        m_select_origin  = origin;
        m_select_last    = last;
        m_select_current = current;

        m_select_state   = Select_Selecting;
    }
    
    void selected(const Point& origin, const Point& last)
    {
        m_select_origin = origin;
        m_select_last   = last;

        m_select_state  = Select_Selected;
    }

    void set_cursor(Gdk::CursorType cursor_type);
    void set_cursor();

    void popup(EventContext* context, Shape* shape, GdkEventButton* event);

    void reset();
private:
    void draw(GdkEventExpose* event=NULL);
    void draw_select(Cairo::RefPtr<Cairo::Context> ctx);
    
    std::vector<EventContext*> m_contexts;
    size_t                     m_current_context;

    enum SelectState {
        Select_None,
        Select_Selecting,
        Select_Selected,
    };

    SelectState m_select_state;
    Point m_select_origin;
    Point m_select_last;
    Point m_select_current;
};

} // namespace DBricks

#endif // DISPLAY_HXX
