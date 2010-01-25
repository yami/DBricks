#ifndef GROUPSHAPE_HXX
#define GROUPSHAPE_HXX

#include <vector>
#include "BoxShape.hxx"

namespace DBricks {

class GroupShape: public BoxShape {
public:
    template<class ForwardIterT>
    GroupShape(ForwardIterT begin, ForwardIterT end)
        :BoxShape((*begin)->bb()), m_shapes(begin, end)
    {
        initialize();
        update_handles();
    }

    GroupShape() {}
    
    virtual void draw_shape(Cairo::RefPtr<Cairo::Context> ctx) const;
    virtual void move(const Point& delta)
    {
        BoxShape::move(delta);
        for (std::vector<Shape*>::const_iterator iter = m_shapes.begin();
             iter != m_shapes.end();
             ++iter) {
            (*iter)->move(delta);
        }        
    }

    virtual Rect bb() const;
    virtual void save(Sml::Object* object) const;
    virtual void load(Sml::Object* object);
private:
    void initialize();
    
    std::vector<Shape*> m_shapes;
};

} // namespace DBricks

#endif // GROUPSHAPE_HXX
