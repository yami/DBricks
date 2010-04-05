#include "Connector.hxx"
#include "Shape.hxx"
#include "IRenderer.hxx"

namespace DBricks {

void
Connector::draw(IRenderer* renderer) const
{
    renderer->save();
    renderer->line_color(Red);
    renderer->draw_line(Point(m_point.x-2, m_point.y-2), Point(m_point.x+2, m_point.y+2));
    renderer->draw_line(Point(m_point.x+2, m_point.y-2), Point(m_point.x-2, m_point.y+2));
    renderer->restore();
}

double
Connector::distance(const Point& point) const
{
    return std::fabs(m_point.x - point.x) + std::fabs(m_point.y - point.y);
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
