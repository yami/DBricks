#include "Connector.hxx"
#include "Shape.hxx"

namespace DBricks {

void
Connector::draw(Cairo::RefPtr<Cairo::Context> ctx) const
{
    ctx->save();
    ctx->set_source_rgba(1, 0, 0, 1);
    ctx->move_to(m_point.x-2, m_point.y-2);
    ctx->line_to(m_point.x+2, m_point.y+2);
    ctx->move_to(m_point.x+2, m_point.y-2);
    ctx->line_to(m_point.x-2, m_point.y+2);
    ctx->stroke();
    ctx->restore();
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
