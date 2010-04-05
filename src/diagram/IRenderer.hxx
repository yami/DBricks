#include "Color.hxx"
#include "LineSpec.hxx"
#include <geom/Point.hxx>
#include <geom/Rect.hxx>

namespace DBricks {

class IRenderer {
public:
    virtual void save() = 0;
    virtual void restore() = 0;
    
    virtual void begin_render(const Rect& update) = 0;
    virtual void end_render() = 0;

    virtual Color background(const Color& color) = 0;

    virtual double line_width(double width) = 0 ;
    virtual Color line_color(const Color& color) = 0;
    virtual LineStyle line_style(LineStyle style) = 0;

    virtual void draw_background(const Rect& area) = 0;
    virtual void draw_line(const Point& from, const Point& to) = 0;
    virtual void draw_rectangle(const Point& top_left, const Point& bottom_right) = 0;
    virtual void draw_ellipse(const Point& center, double width, double height) = 0;
};

} // namespace DBricks
