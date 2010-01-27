#ifndef GTKUTIL_HXX
#define GTKUTIL_HXX

#include <gdk/gdk.h>
#include <geom/Point.hxx>

namespace DBricks {

// extract x, y and set them into point.
// return false if event is not suitable for extracting x, y
bool point_of_event(GdkEvent* event, Point* point);

template<class EventT>
Point point_of_event(EventT* event)
{
    return Point(event->x, event->y);
}

} // namespace DBricks

#endif // GTKUTIL_HXX
