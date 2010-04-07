#ifndef ELLIPSESHAPE_HXX
#define ELLIPSESHAPE_HXX

#include "BoxShape.hxx"

namespace Sml {
class Object;
}

namespace DBricks {

class EllipseShape: public BoxShape {
public:
    EllipseShape() {}
    EllipseShape(const Point& start, Handle*& handle);
    EllipseShape(double x, double y, double width, double height)
        :BoxShape(x, y, width, height)
    {
    }

    virtual void save (Sml::Object* object) const;
    virtual void load (Sml::Object* object);

    virtual EllipseShape* clone() const
    {
        return new EllipseShape(m_x, m_y, m_width, m_height);
    }
private:    
    virtual void draw_shape(IRenderer* renderer) const;
};


class EllipseShapeType : public ShapeType {
public:
    virtual const char* name() const
    {
        return "/standard/ellipse";
    }

    virtual EllipseShape* create(const Point& start, Handle*& handle)
    {
        return new EllipseShape(start, handle);
    }

    virtual EllipseShape* create()
    {
        return new EllipseShape();
    }    
};


} // namespace DBricks

#endif // ELLIPSESHAPE_HXX
