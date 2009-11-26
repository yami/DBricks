#ifndef GROUPSHAPE_HXX
#define GROUPSHAPE_HXX

#include <vector>
#include "BoxShape.hxx"

namespace DBricks {

class GroupShape: public BoxShape {
public:
    template<class ForwardIterT>
    GroupShape(ForwardIterT begin, ForwardIterT end);
    
    virtual void draw_shape(Cairo::RefPtr<Cairo::Context> ctx) const
    {
        for (std::vector<Shape*>::const_iterator iter = m_shapes.begin();
             iter != m_shapes.end();
             ++iter) {
            (*iter)->draw(ctx);
        }
    }

    virtual void move(const Point& delta)
    {
        BoxShape::move(delta);
        for (std::vector<Shape*>::const_iterator iter = m_shapes.begin();
             iter != m_shapes.end();
             ++iter) {
            (*iter)->move(delta);
        }        
    }
private:
    std::vector<Shape*> m_shapes;
};


template<class ForwardIterT>
GroupShape::GroupShape(ForwardIterT begin, ForwardIterT end)
    :BoxShape((*begin)->bb()), m_shapes(begin, end)
{
    double x1 = m_x;
    double y1 = m_y;
    double x2 = m_x + m_width;
    double y2 = m_y + m_height;
    
    for (ForwardIterT iter = begin; iter != end; ++iter) {
        Rect rect = (*iter)->bb();

        x1 = std::min(x1, rect.x1());
        y1 = std::min(y1, rect.y1());
        x2 = std::max(x2, rect.x2());
        y2 = std::max(y2, rect.y2());
    }

    m_x = x1;
    m_y = y1;
    m_width  = x2 - x1;
    m_height = y2 - y1;

    update_handles();
}

} // namespace DBricks

#endif // GROUPSHAPE_HXX
