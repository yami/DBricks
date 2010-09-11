#ifndef ZOOMCONTEXT_HXX
#define ZOOMCONTEXT_HXX

#include "DispatchContext.hxx"
#include <geom/Point.hxx>

namespace DBricks {

class Display;

class ZoomContext: public DispatchContext {
public:
    ZoomContext(Diagram* diagram, Display* display);
private:
    virtual bool on_button_press_event(Shape* shape, GdkEventButton* e);
    virtual bool on_motion_notify_event(Shape* shape, GdkEventMotion* e);
    virtual bool on_button_release_event(Shape* shape, GdkEventButton* e);

    bool m_active;
    bool m_moved;
    Point m_opoint;
    Point m_mpoint;
};

} // namespace DBricks


#endif // ZOOMCONTEXT_HXX
