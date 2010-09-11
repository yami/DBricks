#ifndef IRENDERER_HXX
#define IRENDERER_HXX

#include "Color.hxx"
#include "LineSpec.hxx"
#include <geom/Point.hxx>
#include <geom/Rect.hxx>

#include <vector>
#include <stack>

namespace DBricks {

const int Draw_Stroke = 0x1;
const int Draw_Fill   = 0x2;


enum PathElementType {
    Path_Move_To,
    Path_Line_To,
    Path_Curve_To,
};

struct PathElement {
    enum { Max_Points = 3 };
    PathElementType type;
    Point           points[Max_Points];
};

class IRenderer {
public:
    virtual ~IRenderer() {}
    
    virtual void background(const Color& color);

    virtual void line_width(double width);
    virtual void line_color(const Color& color);
    virtual void line_style(LineStyle style);

    virtual void fill_alpha(double alpha);
    virtual void fill_color(const Color& color);

    
    virtual void begin_render(const Rect& update) = 0;
    virtual void end_render() = 0;

    virtual void draw_background(const Rect& area) = 0;
    virtual void draw_line(const Point& from, const Point& to) = 0;

    // action could be Draw_Stroke or Draw_Fill or 'Draw_Stroke | Draw_Fill'.
    virtual void draw_rectangle(const Point& top_left, const Point& bottom_right, int action) = 0;
    virtual void draw_ellipse(const Point& center, double width, double height, int action) = 0;
    virtual void draw_polygon(const std::vector<Point>& points, int action) = 0;
    virtual void draw_path(const std::vector<PathElement>& elements, int action) = 0;

protected:
    LineSpec m_line_spec;
    FillSpec m_fill_spec;
    Color    m_background;
};

} // namespace DBricks


#endif  // IRENDERER
