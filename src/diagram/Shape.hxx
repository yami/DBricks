#ifndef SHAPE_HXX
#define SHAPE_HXX

#include <vector>
#include <cmath>
#include <geom/Point.hxx>
#include <geom/Rect.hxx>
#include <cairomm/context.h>

#include <util/bit.hxx>

#include "Handle.hxx"
#include "Connector.hxx"

namespace Sml {
class Object;
}


namespace DBricks {

class Connector;

class Menu;

class Shape {
public:
    typedef std::vector<Handle*>    HandlesType;
    typedef std::vector<Connector*> ConnectorsType;
    typedef unsigned int            FlagsType;

    static const FlagsType Break_Connections = 0x1;
    
    Shape(FlagsType flags=0)
        :m_flags(flags), m_corner(0, 0), m_show_handles(false), m_show_connectors(false)
    {
    }

    Shape(double x, double y, FlagsType flags=0)
        :m_flags(flags), m_corner(x, y), m_show_handles(false), m_show_connectors(false)
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

        if (m_show_connectors) {
            for (ConnectorsType::const_iterator iter = m_connectors.begin();
                 iter != m_connectors.end();
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

    virtual Menu* menu(const Point& point) const
    {
        return 0;
    }

    virtual Shape* clone() const
    {
        ASSERT_NOT_REACHED();
        return 0;
    }
    
    HandlesType& handles()
    {
        return m_handles;
    }

    ConnectorsType& connectors()
    {
        return m_connectors;
    }

    bool break_connections() const
    {
        return bit_is_set(m_flags, Break_Connections);
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

    virtual void save (Sml::Object* object) const = 0;
    virtual void load (Sml::Object* object) = 0;
    
private:
    virtual void draw_shape(Cairo::RefPtr<Cairo::Context> ctx) const = 0;
protected:
    HandlesType m_handles;
    ConnectorsType m_connectors;

    FlagsType m_flags;
    
    Point m_corner;
    bool  m_show_handles;
    bool  m_show_connectors;
};

} // namespace DBricks

#endif // SHAPE_HXX
