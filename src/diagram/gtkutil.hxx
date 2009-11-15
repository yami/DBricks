#ifndef GTKUTIL_HXX
#define GTKUTIL_HXX

#include <gdk/gdk.h>

namespace DBricks {

class Point;

// extract x, y and set them into point.
// return false if event is not suitable for extracting x, y
bool point_of_event(GdkEvent* event, Point* point);

} // namespace DBricks

#endif // GTKUTIL_HXX
