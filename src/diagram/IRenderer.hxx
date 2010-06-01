#include "Color.hxx"
#include "LineSpec.hxx"
#include <geom/Point.hxx>
#include <geom/Rect.hxx>
#include <vector>


namespace DBricks {

enum FillAction {
    Fill_Fill,
    Fill_None,
};

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

    virtual double fill_alpha(double alpha) = 0;
    virtual Color fill_color(const Color& color) = 0;    

    virtual void draw_background(const Rect& area) = 0;
    virtual void draw_line(const Point& from, const Point& to) = 0;
    virtual void draw_rectangle(const Point& top_left, const Point& bottom_right, FillAction fill = Fill_None) = 0;
    virtual void draw_ellipse(const Point& center, double width, double height, FillAction fill = Fill_None) = 0;
    virtual void draw_polygon(const std::vector<Point>& points, FillAction fill = Fill_None) = 0;
};

} // namespace DBricks
