#include "Connector.hxx"
#include "Shape.hxx"
#include "IRenderer.hxx"

namespace DBricks {

void
Connector::draw(IRenderer* renderer) const
{
    if (m_mode == Highlight) {
        draw_highlighted(renderer);
    } else {
        if (is_connected())
            draw_connected(renderer);
        else
            draw_normal(renderer);
    }
}

void
Connector::draw_connected(IRenderer* renderer) const
{
    renderer->line_width(1.0);
    renderer->line_color(Black);
    renderer->fill_color(Black);
    renderer->draw_ellipse(Point(m_point.x, m_point.y), 4, 4, Draw_Fill | Draw_Stroke);
}

void
Connector::draw_normal(IRenderer* renderer) const
{
    renderer->line_width(1.0);
    renderer->line_color(Blue);
    renderer->draw_line(Point(m_point.x-2, m_point.y-2), Point(m_point.x+2, m_point.y+2));
    renderer->draw_line(Point(m_point.x+2, m_point.y-2), Point(m_point.x-2, m_point.y+2));
}

void
Connector::draw_highlighted(IRenderer* renderer) const
{
    renderer->line_width(1.0);
    renderer->line_color(Red);
    renderer->fill_color(Red);
    renderer->draw_ellipse(Point(m_point.x, m_point.y), 8, 8, Draw_Stroke | Draw_Fill);
}

double
Connector::distance(const Point& point) const
{
    return std::fabs(m_point.x - point.x) + std::fabs(m_point.y - point.y);
}

void
Connector::build_connections(Connector* a, Connector* b)
{
    a->connect_to(b);
    b->connect_to(a);
}

void
Connector::break_connections(Connector* c)
{
    if (!c)
        return;

    ConnectorsType& connectors = c->connectors();
    for (ConnectorsType::iterator iter = connectors.begin();
         iter != connectors.end();
         ++iter) {
        (*iter)->disconnect(c);
    }

    connectors.clear();
}

} // namespace DBricks
