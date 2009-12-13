#include "Connector.hxx"
#include "Shape.hxx"

namespace DBricks {

void
Connector::draw(Cairo::RefPtr<Cairo::Context> ctx) const
{
    ctx->save();
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
    Point cpoint(m_point+m_shape->corner());
    return std::fabs(cpoint.x - point.x) + std::fabs(cpoint.y - point.y);
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
