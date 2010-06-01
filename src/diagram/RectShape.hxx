#ifndef RECTSHAPE_HXX
#define RECTSHAPE_HXX

#include "BoxShape.hxx"

namespace Sml {
class Object;
}

namespace DBricks {

class Handle;

class RectShape : public BoxShape {
public:
    RectShape() {}
    RectShape(const Point& start, Handle*& handle);
    RectShape(double x, double y, double width, double height)
        :BoxShape(x, y, width, height)
    {
    }

    virtual void save (Sml::Object* object) const;
    virtual void load (Sml::Object* object);

    virtual RectShape* clone() const
    {
        return new RectShape(m_x, m_y, m_width, m_height);
    }

    virtual ShapeType* type() const;
private:
    virtual void draw_shape (IRenderer* renderer) const;
};

} // namespace DBricks

#endif // RECTSHAPE_HXX
