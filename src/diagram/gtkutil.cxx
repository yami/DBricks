#include "gtkutil.hxx"
#include <util/assert.hxx>
#include <geom/Point.hxx>

namespace DBricks {

template<class EventT>
static Point
event_point(GdkEvent* event)
{
    EventT* e = reinterpret_cast<EventT*>(event);
    return Point(e->x, e->y);
}


bool point_of_event(GdkEvent* event, Point* point)
{
    if (event->type == GDK_BUTTON_PRESS |
        event->type == GDK_2BUTTON_PRESS |
        event->type == GDK_3BUTTON_PRESS |
        event->type == GDK_BUTTON_RELEASE) {
        *point = event_point<GdkEventButton>(event);
        return true;
    }

    if (event->type == GDK_MOTION_NOTIFY) {
        *point = event_point<GdkEventMotion>(event);
        return true;
    }

    if (event->type == GDK_ENTER_NOTIFY |
        event->type == GDK_LEAVE_NOTIFY) {
        *point = event_point<GdkEventCrossing>(event);
        return true;
    }

    return false;
}

} // namespace DBricks
