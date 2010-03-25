#include "Handle.hxx"
#include "Shape.hxx"

namespace DBricks {

void
Handle::draw(Cairo::RefPtr<Cairo::Context> ctx) const
{
    const double hw = 4;
    ctx->save();
    ctx->set_source_rgba(0.337, 0.612, 0.117, 0.9);
    ctx->rectangle(m_point.x-hw, m_point.y-hw, hw*2, hw*2);
    ctx->stroke();
    ctx->fill();               // fill with color
    ctx->restore();
}

} // namespace DBricks
