#include "RectShape.hxx"

namespace DBricks {

void
RectShape::draw_shape(Cairo::RefPtr<Cairo::Context> ctx) const
{
    ctx->save();
    ctx->rectangle(m_x, m_y, m_width, m_height);

    ctx->set_source_rgba(1, 1, 1, 1); // set to white
    ctx->fill_preserve();

    ctx->set_source_rgba(0.117, 0.337, 0.612, 1); // set to blue
    ctx->stroke();
    
    ctx->restore();
}

} // namespace DBricks
