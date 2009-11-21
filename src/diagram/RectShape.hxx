#ifndef RECTSHAPE_HXX
#define RECTSHAPE_HXX

#include "BoxShape.hxx"


namespace DBricks {

class RectShape : public BoxShape {
public:
    RectShape(double x, double y, double width, double height)
        :BoxShape(x, y, width, height)
    {
    }

private:
    virtual void draw_shape (Cairo::RefPtr<Cairo::Context> ctx) const;
};

} // namespace DBricks

#endif // RECTSHAPE_HXX
