#ifndef CONNECTOR_HXX
#define CONNECTOR_HXX

#include <cmath>

#include <util/assert.hxx>
#include <util/stl.hxx>

#include <geom/Point.hxx>
#include <cairomm/context.h>

namespace DBricks {

class Shape;

// A connects to B, B connects to C, then A connects to C.
class Connector {
public:

    enum ConnectorKind {
        Passive,
        Active,
    };
    
    Connector(Shape* shape, const Point& point, ConnectorKind kind = Passive)
        :m_shape(shape), m_point(point), m_kind(kind)
    {
    }
    
    typedef std::vector<Connector*> ConnectorsType;
    
    static void build_connections(Connector* a, Connector* b)
    {
        a->connect_to(b);
        b->connect_to(a);
    }

    static void break_connections(Connector* c);
    
    void connect_to(Connector* c)
    {
        ASSERT(!util::in_container(m_connectors, c));
        m_connectors.push_back(c);
    }

    void disconnect(Connector* c)
    {
        ASSERT(util::in_container(m_connectors, c));
        util::delete_value(m_connectors, c);
    }

    ConnectorsType& connectors()
    {
        return m_connectors;
    }

    const Point& point() const
    {
        return m_point;
    }

    void point(const Point& point)
    {
        m_point = point;
    }

    double distance(const Point& point) const;

    Shape* shape() const
    {
        return m_shape;
    }

    void draw(Cairo::RefPtr<Cairo::Context> ctx) const;

    bool is_active() const
    {
        return m_kind == Active;
    }
private:
    Shape*         m_shape;
    ConnectorsType m_connectors;
    Point          m_point;
    ConnectorKind  m_kind;
};

} // namespace DBricks

#endif // CONNECTOR_HXX
