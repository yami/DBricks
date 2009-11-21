#include "DispatchContext.hxx"

namespace DBricks {

bool
DispatchContext::on_event(Shape* shape, GdkEvent *event)
{
    bool pass_down = false;
    
    switch (event->type) {
        case GDK_BUTTON_PRESS:
            return on_button_press_event(shape, reinterpret_cast<GdkEventButton*>(event));
        case GDK_BUTTON_RELEASE:
            return on_button_release_event(shape, reinterpret_cast<GdkEventButton*>(event));
        case GDK_MOTION_NOTIFY:
            return on_motion_notify_event(shape, reinterpret_cast<GdkEventMotion*>(event));
        default:
            pass_down = true;
    }

    return pass_down;
}

} // namespace DBricks
