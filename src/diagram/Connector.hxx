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
    Connector(Shape* shape, const Point& point)
        :m_shape(shape), m_point(point)
    {
    }
    
    typedef std::vector<Connector*> ConnectorsType;
    
    static void connect(Connector* a, Connector* b)
    {
        a->connect_to(b);
        b->connect_to(a);
    }
    
    void connect_to(Connector* c)
    {
        ASSERT(!util::in_container(m_connectors, c));
        m_connectors.push_back(c);
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
    
private:
    Shape*         m_shape;
    ConnectorsType m_connectors;
    Point          m_point;
};

} // namespace DBricks

#endif // CONNECTOR_HXX
