#include "Handle.hxx"
#include "Shape.hxx"

namespace DBricks {

void
Handle::draw(Cairo::RefPtr<Cairo::Context> ctx) const
{
    const double hw = 4;
    ctx->save();
    ctx->rectangle(m_shape->corner().x+m_point.x-hw, m_shape->corner().y+m_point.y-hw, hw*2, hw*2);
    ctx->stroke();
    ctx->fill();               // fill with color
    ctx->restore();
}

} // namespace DBricks
