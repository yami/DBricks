#include "gtkutil.hxx"
#include <geom/Point.hxx>

namespace DBricks {

template<class EventT>
static bool
event_point(GdkEvent* event, Point* point)
{
    EventT* e = reinterpret_cast<EventT*>(event);
    point->x = e->x;
    point->y = e->y;

    return true;
}


bool point_of_event(GdkEvent* event, Point* point)
{
    if (event->type == GDK_BUTTON_PRESS |
        event->type == GDK_2BUTTON_PRESS |
        event->type == GDK_3BUTTON_PRESS |
        event->type == GDK_BUTTON_RELEASE) {
        return event_point<GdkEventButton>(event, point);
    }

    if (event->type == GDK_MOTION_NOTIFY) {
        return event_point<GdkEventMotion>(event, point);
    }

    if (event->type == GDK_ENTER_NOTIFY |
        event->type == GDK_LEAVE_NOTIFY) {
        return event_point<GdkEventCrossing>(event, point);
    }

    return false;
}

} // namespace DBricks
