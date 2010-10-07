#include "ZoomWindow.hxx"

#include <geom/computation.hxx>

namespace DBricks {

ZoomWindow::ZoomWindow(const Rect& visible, double factor)
    :m_visible(visible), m_factor(factor)
{
}

double ZoomWindow::to_display_length(double length) const
{
    return m_factor * length;
}

Point ZoomWindow::to_display_coord(const Point& point) const
{
    return Point(round((point.x - m_visible.x1()) * m_factor),
                 round((point.y - m_visible.y1()) * m_factor));
}

Rect ZoomWindow::to_display_rect(const Rect& rect) const
{
    return Rect(to_display_coord(Point(rect.x1(), rect.y1())),
                to_display_coord(Point(rect.x2(), rect.y2())));
}

double ZoomWindow::to_real_length(double length) const
{
    return length/m_factor;
}

Point ZoomWindow::to_real_coord(const Point& point) const
{
    return Point(round(point.x/m_factor + m_visible.x1()),
                 round(point.y/m_factor + m_visible.y1()));
}

Rect ZoomWindow::to_real_rect(const Rect& rect) const
{
    return Rect(to_real_coord(Point(rect.x1(), rect.y1())),
                to_real_coord(Point(rect.x2(), rect.x2())));
}

double ZoomWindow::x1() const
{
    return m_visible.x1();
}

double ZoomWindow::y1() const
{
    return m_visible.y1();
}

double ZoomWindow::x2() const
{
    return m_visible.x2();
}

double ZoomWindow::y2() const
{
    return m_visible.y2();
}

double ZoomWindow::width() const
{
    return m_visible.width();
}

double ZoomWindow::height() const
{
    return m_visible.height();
}

void ZoomWindow::set(double factor, const Rect& visible)
{
    if (double_equal(m_factor, factor) && m_visible == visible)
        return;
    
    m_factor  = factor;
    m_visible = visible;

    m_signal_changed.emit(*this);
}

double ZoomWindow::factor() const
{
    return m_factor;
}

sigc::signal<void, const ZoomWindow&>& ZoomWindow::signal_changed()
{
    return m_signal_changed;
}

} // namespace DBricks
