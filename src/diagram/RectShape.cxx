#include "RectShape.hxx"

namespace DBricks {

void
RectShape::draw(Cairo::RefPtr<Cairo::Context> ctx) const
{
    ctx->save();
    ctx->rectangle(m_x, m_y, m_width, m_height);
    ctx->stroke();
    ctx->restore();
}

} // namespace DBricks
