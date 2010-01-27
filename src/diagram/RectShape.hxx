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
private:
    virtual void draw_shape (Cairo::RefPtr<Cairo::Context> ctx) const;
};

} // namespace DBricks

#endif // RECTSHAPE_HXX
