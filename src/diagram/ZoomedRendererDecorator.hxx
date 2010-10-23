#ifndef ZOOMEDRENDERERDECORATOR_HXX
#define ZOOMEDRENDERERDECORATOR_HXX

#include "IRenderer.hxx"

namespace DBricks {

class ZoomWindow;

class ZoomedRendererDecorator : public IRenderer {
public:
    ZoomedRendererDecorator(IRenderer* backedRenderer, ZoomWindow* zwindow)
        :m_backedRenderer(backedRenderer), m_zwindow(zwindow)
    {
    }

    virtual void begin_render(const Rect& update);
    virtual void end_render();
    
    virtual void line_width(double width);

    virtual void draw_background(const Rect& area);
    virtual void draw_line(const Point& from, const Point& to);

    virtual void draw_rectangle(const Point& top_left, const Point& bottom_right, int action);
    virtual void draw_ellipse(const Point& center, double width, double height, int action);
    virtual void draw_polygon(const std::vector<Point>& points, int action);
    virtual void draw_path(const std::vector<PathElement>& elements, int action);

    virtual void draw_text(const TextString& text, int action);
private:
    IRenderer*  m_backedRenderer;
    ZoomWindow* m_zwindow;
};

} // namespace DBricks

#endif // ZOOMEDRENDERERDECORATOR_HXX
