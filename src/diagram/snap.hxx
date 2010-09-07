#ifndef SNAP_HXX
#define SNAP_HXX

#include <geom/Point.hxx>

namespace DBricks {

void snap_set_grid(double width, double height);
Point snap(const Point& source);

}


#endif
