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
    //CairoRenderer(Cairo::RefPtr<Cairo::Context> ctx);
    CairoRenderer(Glib::RefPtr<Gdk::Window> window);
    CairoRenderer();
    
    virtual void save();
    virtual void restore();
    
    virtual void begin_render(const Rect& update);
    virtual void end_render();

    virtual Color background(const Color& color);
    virtual double line_width(double width);
    virtual Color line_color(const Color& color);
    virtual LineStyle line_style(LineStyle style);
    
    virtual void draw_background(const Rect& update);
        
    virtual void draw_line(const Point& from, const Point& to);
    virtual void draw_rectangle(const Point& top_left, const Point& bottom_right);
    virtual void draw_ellipse(const Point& center, double width, double height);

private:
    Color background__(const Color& color);
    double line_width__(double width);
    Color line_color__(const Color& color);
    LineStyle line_style__(LineStyle style);
    
    enum ModificationType {
        MT_LineColor,
        MT_LineWidth,
        MT_LineStyle,
        MT_BackgroundColor,
    };
    
    class RendererModification {
    public:
    
        RendererModification(ModificationType t, void* d)
            :type(t), data(0)
        {
            data = new char[modification_size(type)];
            
            switch (type) {
                case MT_LineWidth:
                {
                    double* value = (double *)data;
                    *value = *(double *)d;
                    break;
                }
                case MT_LineColor:
                case MT_BackgroundColor:
                {
                    Color* value = (Color *)data;
                    *value = *(Color *)d;
                    break;
                }
                case MT_LineStyle:
                {
                    LineStyle* value = (LineStyle *)data;
                    *value = *(LineStyle *)d;
                    break;
                }
            }
        }

        ~RendererModification()
        {
            delete[] data;
        }
    
        static size_t modification_size(ModificationType t)
        {
            switch (t) {
                case MT_LineColor:
                case MT_BackgroundColor:
                    return sizeof(Color);  break;
                case MT_LineWidth:
                    return sizeof(double); break;
                case MT_LineStyle:
                    return sizeof(LineStyle); break;
            }
        }
    
        ModificationType  type;
        char*             data;
    };

    
    void do_change(Change* change);

    void save_to_stack(ModificationType type, void* data);

    Glib::RefPtr<Gdk::Window>     m_window;
    Cairo::RefPtr<Cairo::Context> m_ctx;
    LineSpec                      m_line_spec;
    Color                         m_background;

    std::stack<std::vector<RendererModification*> > m_save_stack;
};

}


#endif // CAIRORENDERER_HXX
