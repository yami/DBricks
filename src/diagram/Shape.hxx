#ifndef SHAPE_HXX
#define SHAPE_HXX

#include <vector>
#include <cmath>
#include <geom/Point.hxx>
#include <geom/Rect.hxx>
#include <cairomm/context.h>

#include "Handle.hxx"

namespace DBricks {

class Connector;

class Shape {
public:
    typedef std::vector<Handle*> HandlesType;
    typedef std::vector<Connector*> ConnectorsType;
    
    Shape()
        :m_corner(0, 0), m_show_handles(false), m_show_connectors(false)
    {
    }

    Shape(double x, double y)
        :m_corner(x, y), m_show_handles(false), m_show_connectors(false)
    {
    }
    
    ~Shape()
    {
    }

    void draw (Cairo::RefPtr<Cairo::Context> ctx) const
    {
        draw_shape(ctx);

        if (m_show_handles) {
            for (HandlesType::const_iterator iter = m_handles.begin();
                 iter != m_handles.end();
                 ++iter) {
                (*iter)->draw(ctx);
            }
        }
    }

    virtual void move_handle(Handle* handle, const Point& delta) = 0;
    virtual void move_connector(Connector* connector, const Point& delta) = 0;
    
    virtual void move(const Point& delta) = 0;

    virtual double distance(const Point& point) const
    {
        return fabs(m_corner.x - point.x) + fabs(m_corner.y - point.y);
    }

    virtual bool cover(const Point& point) const = 0;
    virtual bool in(const Rect& rect) const = 0;
    virtual Rect bb() const = 0;
    
    HandlesType& handles()
    {
        return m_handles;
    }

    ConnectorsType& connectors()
    {
        return m_connectors;
    }

    const Point& corner() const
    {
        return m_corner;
    }

    void show_handles()
    {
        m_show_handles = true;
    }

    void hide_handles()
    {
        m_show_handles = false;
    }

    void show_connectors()
    {
        m_show_connectors = true;
    }

    void hide_connectors()
    {
        m_show_connectors = false;
    }
    
private:
    virtual void draw_shape(Cairo::RefPtr<Cairo::Context> ctx) const = 0;
protected:
    HandlesType m_handles;
    ConnectorsType m_connectors;
    
    Point m_corner;
    bool  m_show_handles;
    bool  m_show_connectors;
};

} // namespace DBricks

#endif // SHAPE_HXX
