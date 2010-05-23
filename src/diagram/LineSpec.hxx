#ifndef LINESPEC_HXX
#define LINESPEC_HXX

#include "Color.hxx"
#include <cassert>

namespace DBricks {

enum LineStyle {
    LS_None,
    LS_Solid,
    LS_Dash,
};

struct LineSpec {
    double    width;
    Color     color;
    LineStyle style;
};

struct FillSpec {
    Color  color;
    double alpha;
};

} // namespace DBricks


#endif // LINESPEC_HXX
