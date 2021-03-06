#ifndef DISPLAY_HXX
#define DISPLAY_HXX

#include <vector>

#include <gtkmm/drawingarea.h>
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
class IRenderer;
class Desktop;
class ZoomWindow;

class Display : public Gtk::DrawingArea, public DiagramObserver {
public:
    Display(Diagram* diagram, Desktop* desktop);
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

    void set_context(EventContext* context);

    void set_highlight_closest_connector()
    {
        m_highlight_closest_connector = true;
    }

    ZoomWindow* zwindow() const
    {
        return m_zwindow;
    }

    enum ZoomPoint {
        ZoomPoint_Center,
        ZoomPoint_Origin,
    };
    
    void zoom(const Point& center, double factor, ZoomPoint zp);

private:
    void draw(GdkEventExpose* event=NULL);
    void draw_select();
    void draw_grid(int width, int height);
        
    Desktop*      m_desktop;
    EventContext* m_context;
    
    enum SelectState {
        Select_None,
        Select_Selecting,
        Select_Selected,
    };

    SelectState m_select_state;
    Point m_select_origin;
    Point m_select_last;
    Point m_select_current;

    IRenderer* m_renderer;

    bool m_highlight_closest_connector;
    GdkEvent*  m_event;

    ZoomWindow* m_zwindow;
};

} // namespace DBricks

#endif // DISPLAY_HXX
