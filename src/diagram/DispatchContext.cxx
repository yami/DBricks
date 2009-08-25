#include "DispatchContext.hxx"

namespace DBricks {

bool
DispatchContext::on_event(GdkEvent *event)
{
    bool pass_down = false;

    switch (event->type) {
        case GDK_BUTTON_PRESS:
            pass_down = on_button_press_event(reinterpret_cast<GdkEventButton*>(event));
            break;
        case GDK_MOTION_NOTIFY:
            pass_down = on_motion_notify_event(reinterpret_cast<GdkEventMotion*>(event));
            break;
        default:
            pass_down = true;
    }

    return pass_down;
}

} // namespace DBricks
