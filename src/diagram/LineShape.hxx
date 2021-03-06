#ifndef LINESHAPE_HXX
#define LINESHAPE_HXX

#include "Shape.hxx"
#include "Connector.hxx"

namespace DBricks {

class Handle;

class LineShape: public Shape {
public:
    LineShape();
    LineShape(const Point& start, Handle*& handle);
    LineShape(const Point& from, const Point& to);
    
    // Shape interfaces
    virtual void draw_shape (IRenderer* renderer) const;
    virtual void move_handle(Handle* handle, const Point& delta);
    virtual void move(const Point& delta);
    virtual double distance(const Point& point) const;
    virtual bool cover(const Point& point) const;
    virtual bool in(const Rect& rect) const;
    virtual Rect bb() const;

    virtual void save (DiagramArchiver* ar) const;
    virtual void load (DiagramArchiver* ar);

    virtual LineShape* clone() const
    {
        return new LineShape(m_fhandle.point(), m_thandle.point());
    }

    virtual Gtk::Widget* property_widget();
    virtual void         property_apply();

    virtual ShapeType* type() const;
private:
    void initialize(const Point& from, const Point& to);
    
    Connector m_fconnector;
    Connector m_tconnector;
    
    Handle m_fhandle;
    Handle m_thandle;

    // temp hack
    Point  m_from;
    Point  m_to;
};

} // namespace DBricks

#endif // LINESHAPE_HXX
