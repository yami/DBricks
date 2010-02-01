#ifndef HANDLE_HXX
#define HANDLE_HXX

#include <string>
#include <cmath>
#include <geom/Point.hxx>
#include <cairomm/context.h>

#include <util/assert.hxx>

namespace DBricks {

class Shape;
class Connector;

class Handle {
public:
    Handle() {}
    
    Handle(Shape* shape, const Point& point)
        :m_shape(shape), m_connector(0), m_point(point)
    {
    }

    Handle(Shape* shape, Connector* connector, const Point& point)
        :m_shape(shape), m_connector(connector), m_point(point)
    {
        ASSERT(m_connector);
    }

    ~Handle()
    {
    }

    void initialize(Shape* shape, Connector* connector, const Point& point)
    {
        m_shape     = shape;
        m_connector = connector;
        m_point     = point;
    }    

    Connector* connector() const
    {
        return m_connector;
    }

    const Point& point() const
    {
        return m_point;
    }

    void point(const Point& point)
    {
        m_point = point;
    }
    
    double distance(const Point& point) const
    {
        return std::fabs(m_point.x - point.x) + std::fabs(m_point.y - point.y);
    }

    void draw(Cairo::RefPtr<Cairo::Context> ctx) const;

    Shape* shape() const
    {
        return m_shape;
    }
private:
    Shape*      m_shape;
    Connector*  m_connector;
    Point       m_point;
};

} // namespace diagram

#endif  // HANDLE_HXX
