#include "GroupShape.hxx"
#include <algorithm>

#include "Archiver.hxx"

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

void
GroupShape::serialize(Archiver* ar) const
{
    ar->object_begin("Group");
    ar->list_begin("GroupItems");
    std::for_each(m_shapes.begin(), m_shapes.end(),
                  std::bind2nd(std::mem_fun(&Shape::serialize), ar));
    ar->list_end("GroupItems");
    ar->object_end("Group");
}


} // namespace DBricks
