#include "ZoomedRendererDecorator.hxx"
#include "ZoomWindow.hxx"

namespace DBricks {

void ZoomedRendererDecorator::begin_render(const Rect& update)
{
    m_backedRenderer->begin_render(m_zwindow->to_display_rect(update));
}

void ZoomedRendererDecorator::end_render()
{
    m_backedRenderer->end_render();
}


void ZoomedRendererDecorator::line_width(double width)
{
    m_backedRenderer->line_width(m_zwindow->to_display_length(width));
}

void ZoomedRendererDecorator::draw_background(const Rect& area)
{
    m_backedRenderer->draw_background(m_zwindow->to_display_rect(area));
}

void ZoomedRendererDecorator::draw_line(const Point& from, const Point& to)
{
    m_backedRenderer->draw_line(
        m_zwindow->to_display_coord(from),
        m_zwindow->to_display_coord(to));
}

void ZoomedRendererDecorator::draw_rectangle(const Point& top_left, const Point& bottom_right, int action)
{
    m_backedRenderer->draw_rectangle(
        m_zwindow->to_display_coord(top_left),
        m_zwindow->to_display_coord(bottom_right),
        action);
}

void ZoomedRendererDecorator::draw_ellipse(const Point& center, double width, double height, int action)
{
    m_backedRenderer->draw_ellipse(
        m_zwindow->to_display_coord(center),
        m_zwindow->to_display_length(width),
        m_zwindow->to_display_length(height),
        action);
}

void ZoomedRendererDecorator::draw_polygon(const std::vector<Point>& points, int action)
{
    std::vector<Point> display_points(points.size());
    for (size_t i=0; i<points.size(); ++i) {
        display_points[i] = m_zwindow->to_display_coord(points[i]);
    }

    m_backedRenderer->draw_polygon(display_points, action);
}

void ZoomedRendererDecorator::draw_path(const std::vector<PathElement>& elements, int action)
{
    std::vector<PathElement> display_elements(elements);
    for (size_t e = 0; e < display_elements.size(); ++e) {
        for (int i = 0; i < PathElement::Max_Points; ++i) {
            display_elements[e].points[i] = m_zwindow->to_display_coord(elements[e].points[i]);
        }
    }

    m_backedRenderer->draw_path(display_elements, action);
}

} // namespace DBricks
