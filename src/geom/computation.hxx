#ifndef COMPUTATION_HXX
#define COMPUTATION_HXX

#include <cmath>
#include "Point.hxx"

namespace DBricks {

// distance of p0 to line (p1, p2)
// |(x0 - x2)(y1 - y2) - (x1 - x2)(y0 - y2)|
// -----------------------------------------
//                                  1/2
//     [(y1 - y2)**2 + (x1 - x2)**2]
double point_line_distance(const Point& p0, const Point& p1, const Point& p2)
{
    double x1_x2 = p1.x - p2.x;
    double y1_y2 = p1.y - p2.y;
    double x0_x2 = p0.x - p2.x;
    double y0_y2 = p0.y - p2.y;

    return std::fabs(x0_x2*y1_y2 - x1_x2*y0_y2) / std::sqrt(y1_y2*y1_y2 + x1_x2*x1_x2);
}

} // namespace DBricks


#endif // COMPUTATION_HXX
