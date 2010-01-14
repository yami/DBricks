#ifndef LINESHAPE_HXX
#define LINESHAPE_HXX

#include "Shape.hxx"
#include "Connector.hxx"

namespace DBricks {

class Archiver;

class LineShape: public Shape {
public:
    LineShape(const Point& from, const Point& to);
    
    // Shape interfaces
    virtual void draw_shape (Cairo::RefPtr<Cairo::Context> ctx) const;
    virtual void move_handle(Handle* handle, const Point& delta);
    virtual void move_connector(Connector* connector, const Point& delta);
    virtual void move(const Point& delta);
    virtual double distance(const Point& point) const;
    virtual bool cover(const Point& point) const;
    virtual bool in(const Rect& rect) const;
    virtual Rect bb() const;

    virtual void serialize(Archiver* ar) const;
private:
    Connector m_fconnector;
    Connector m_tconnector;
    
    Handle m_fhandle;
    Handle m_thandle;
};

} // namespace DBricks

#endif // LINESHAPE_HXX
