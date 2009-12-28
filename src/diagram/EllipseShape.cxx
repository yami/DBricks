#include "EllipseShape.hxx"


namespace DBricks {

void
EllipseShape::draw_shape(Cairo::RefPtr<Cairo::Context> ctx) const
{
    ctx->save();

    ctx->translate(m_x+m_width/2, m_y+m_height/2);
    ctx->scale(m_width, m_height);

    ctx->set_line_width(0.025);

    ctx->begin_new_sub_path();  // avoid extra line...
    ctx->arc(0, 0, 0.5, 0.0, 2 * M_PI);

    ctx->set_source_rgba(1, 1, 1, 1); // set to white
    ctx->fill_preserve();

    ctx->set_source_rgba(0.117, 0.337, 0.612, 1); // set to blue    
    ctx->stroke();
    ctx->restore();
}


} // namespace DBricks
