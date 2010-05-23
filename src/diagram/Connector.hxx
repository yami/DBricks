#ifndef CONNECTOR_HXX
#define CONNECTOR_HXX

#include <cmath>

#include <util/assert.hxx>
#include <util/stl.hxx>

#include <geom/Point.hxx>
#include <cairomm/context.h>

namespace DBricks {

class Shape;
class IRenderer;

// A connects to B, B connects to C, then A connects to C.
class Connector {
public:

    enum ConnectorKind {
        Passive,
        Active,
    };

    enum DrawMode {
        Normal,
        Highlight,
    };
    
    Connector() {}
    
    Connector(Shape* shape, const Point& point, ConnectorKind kind = Passive)
        :m_shape(shape), m_point(point), m_last_point(point), m_kind(kind), m_mode(Normal)
    {
    }

    void initialize(Shape* shape, const Point& point, ConnectorKind kind = Passive)
    {
        m_shape      = shape;
        m_point      = point;
        m_last_point = point;
        m_kind       = kind;
    }
    
    typedef std::vector<Connector*> ConnectorsType;
    
    static void build_connections(Connector* a, Connector* b);
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
        m_last_point = m_point;
        m_point = point;
    }

    const Point& last_point() const
    {
        return m_last_point;
    }

    void reset_last_point()
    {
        m_last_point = m_point;
    }
    
    double distance(const Point& point) const;
    
    Shape* shape() const
    {
        return m_shape;
    }

    void draw(IRenderer* renderer) const;

    bool is_active() const
    {
        return m_kind == Active;
    }

    bool is_connected() const
    {
        return !m_connectors.empty();
    }

    void highlight(bool h)
    {
        m_mode = h ? Highlight : Normal;
    }
private:
    void draw_normal(IRenderer* renderer) const;
    void draw_connected(IRenderer* renderer) const;
    void draw_highlighted(IRenderer* renderer) const;
    
    Shape*         m_shape;
    ConnectorsType m_connectors;
    Point          m_point;
    Point          m_last_point;
    ConnectorKind  m_kind;
    DrawMode       m_mode;
};


} // namespace DBricks

#endif // CONNECTOR_HXX
