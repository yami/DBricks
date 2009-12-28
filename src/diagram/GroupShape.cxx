#include "GroupShape.hxx"
#include <algorithm>

namespace DBricks {

void
GroupShape::draw_shape(Cairo::RefPtr<Cairo::Context> ctx) const
{
    for (std::vector<Shape*>::const_iterator iter = m_shapes.begin();
         iter != m_shapes.end();
         ++iter) {
        (*iter)->draw(ctx);
    }
    
    if (m_show_handles || m_show_connectors) {
        Rect rect = bb();
        ctx->save();
        ctx->rectangle(rect.x1(), rect.y1(), rect.width(), rect.height());
        ctx->stroke();
        ctx->restore();
    }
}

Rect
GroupShape::bb() const
{
    std::vector<Shape*>::const_iterator iter = m_shapes.begin();
    Rect rect((*iter)->bb());
    for (++iter; iter != m_shapes.end(); ++iter) {
        Rect curr((*iter)->bb());
        rect = Rect(std::min(rect.x1(), curr.x1()),
                    std::min(rect.y1(), curr.y1()),
                    std::max(rect.x2(), curr.x2()),
                    std::max(rect.y2(), curr.y2()));
    }

    return rect;
}


} // namespace DBricks
