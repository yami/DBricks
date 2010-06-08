#ifndef IRENDERER_HXX
#define IRENDERER_HXX

#include "Color.hxx"
#include "LineSpec.hxx"
#include <geom/Point.hxx>
#include <geom/Rect.hxx>
#include <vector>


namespace DBricks {

const int Draw_Stroke = 0x1;
const int Draw_Fill   = 0x2;


enum PathElementType {
    Path_Move_To,
    Path_Line_To,
    Path_Curve_To,
};

struct PathElement {
    PathElementType type;
    Point           points[3];
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

    // action could be Draw_Stroke or Draw_Fill or 'Draw_Stroke | Draw_Fill'.
    virtual void draw_rectangle(const Point& top_left, const Point& bottom_right, int action) = 0;
    virtual void draw_ellipse(const Point& center, double width, double height, int action) = 0;
    virtual void draw_polygon(const std::vector<Point>& points, int action) = 0;
    virtual void draw_path(const std::vector<PathElement>& elements, int action) = 0;
};

} // namespace DBricks


#endif  // IRENDERER
