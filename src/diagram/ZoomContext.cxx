#include "ZoomContext.hxx"
#include "ZoomWindow.hxx"
#include "Display.hxx"

#include "defines.hxx"
#include "gtkutil.hxx"

#include <algorithm>
using std::min;
using std::max;

namespace DBricks {

ZoomContext::ZoomContext(Diagram* diagram ,Display* display)
    :DispatchContext(diagram, display), m_active(false), m_moved(false)
{
}

bool
ZoomContext::on_button_press_event(Shape* shape, GdkEventButton* e)
{
    if (e->button == Left_Button || e->button == Right_Button) {
        m_active = true;
        m_moved  = false;
        m_opoint = point_of_event(e);
        m_mpoint = m_opoint;
    }

    return false;
}

bool
ZoomContext::on_motion_notify_event(Shape* shape, GdkEventMotion* e)
{
    if (!m_active)
        return false;
    
    Point current = point_of_event(e);
    m_moved = true;
    m_display->selecting(m_opoint, m_mpoint, current);

    return false;
}

bool
ZoomContext::on_button_release_event(Shape* shape, GdkEventButton* e)
{
    if (!m_active)
        return false;
    
    Point current = point_of_event(e);
    bool  larger  = e->button == Left_Button;
    
    m_display->selected(m_opoint, current);

    const ZoomWindow* zwindow = m_display->zwindow();

    Point real_opoint  = zwindow->to_real_coord(m_opoint);
    Point real_current = zwindow->to_real_coord(current);

    Point origin;
    origin.x = min(real_opoint.x, real_current.x);
    origin.y = min(real_opoint.y, real_current.y);

    double factor;
    
    if (m_moved) {        
        double hdiff = fabs(real_current.x - real_opoint.x);
        double vdiff = fabs(real_current.y - real_opoint.y);

        if (larger) {
            double hfactor = zwindow->width()/hdiff;
            double vfactor = zwindow->height()/vdiff;

            factor = min(hfactor, vfactor);
        } else {
            double hfactor = hdiff / zwindow->width();
            double vfactor = vdiff / zwindow->height();

            factor  = max(hfactor, vfactor);
            
            origin.x *= (1.0 + factor);
            origin.y *= (1.0 + factor);
        }
    } else {
        factor = larger ? 2.0 : 0.5;
    }

    m_display->zoom(origin, factor);
    
    m_active = false;
}

} // namespace DBricks
