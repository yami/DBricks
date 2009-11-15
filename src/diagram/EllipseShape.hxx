#ifndef ELLIPSESHAPE_HXX
#define ELLIPSESHAPE_HXX

#include "BoxShape.hxx"

namespace DBricks {

class EllipseShape: public BoxShape {
public:
    EllipseShape(double x, double y, double width, double height)
        :BoxShape(x, y, width, height)
    {
    }
    
    virtual void draw (Cairo::RefPtr<Cairo::Context> ctx) const;
};

} // namespace DBricks

#endif // ELLIPSESHAPE_HXX
