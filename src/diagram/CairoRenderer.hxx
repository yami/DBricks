#ifndef CAIRORENDERER_HXX
#define CAIRORENDERER_HXX

#include "IRenderer.hxx"
#include "Color.hxx"
#include <cairomm/context.h>
#include <stack>

#include <gtkmm.h>

namespace DBricks {

class ChangeSet;
class Change;

class CairoRenderer: public IRenderer {
public:
    CairoRenderer(Glib::RefPtr<Gdk::Window> window);
    ~CairoRenderer();
    
    virtual void begin_render(const Rect& update);
    virtual void end_render();

    virtual void draw_background(const Rect& update);        
    virtual void draw_line(const Point& from, const Point& to);
    
    virtual void draw_rectangle(const Point& top_left, const Point& bottom_right, int action);
    virtual void draw_ellipse(const Point& center, double width, double height, int action);
    virtual void draw_polygon(const std::vector<Point>& points, int action);
    virtual void draw_path(const std::vector<PathElement>& elements, int action);

    virtual void draw_text(const TextString& text, int action);
    
private:
    void set_line_spec();
    void set_fill_spec();
    
    void do_draw_action(int action);    
    
    Glib::RefPtr<Gdk::Window>     m_window;
    Cairo::RefPtr<Cairo::Context> m_ctx;
    PangoLayout*                  m_layout;
};

}


#endif // CAIRORENDERER_HXX
