#ifndef ZOOMWINDOW_HXX
#define ZOOMWINDOW_HXX

#include <geom/Point.hxx>
#include <geom/Rect.hxx>

#include <sigc++/sigc++.h>

namespace DBricks {

class ZoomWindow {
public:
    ZoomWindow(const Rect& visible, double factor);
    
    double to_display_length(double length) const;
    Point  to_display_coord(const Point& point) const;
    Rect   to_display_rect(const Rect& rect) const;

    double to_real_length(double length) const;
    Point  to_real_coord(const Point& point) const;
    Rect   to_real_rect(const Rect& rect) const;

    double x1() const;
    double y1() const;
    double x2() const;
    double y2() const;

    double width() const;
    double height() const;
    
    Rect   visible() const;
    double factor() const;

    void set(double factor, const Rect& visible);

    sigc::signal<void, const ZoomWindow&>& signal_changed();
private:
    Rect   m_visible;           // visible area: rectangle in real
    double m_factor;            // zoom factor = visible/real

    sigc::signal<void, const ZoomWindow&> m_signal_changed;
};



} // namespace DBricks


#endif // ZOOMWINDOW_HXX
