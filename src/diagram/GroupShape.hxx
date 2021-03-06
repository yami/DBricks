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

    virtual void draw_shape(IRenderer* renderer) const;
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
    virtual void save(DiagramArchiver* ar) const;
    virtual void load(DiagramArchiver* ar);

    virtual GroupShape* clone() const
    {
        std::vector<Shape*> shapes;
        for (std::vector<Shape*>::const_iterator iter = m_shapes.begin();
             iter != m_shapes.end();
             ++iter) {
            shapes.push_back((*iter)->clone());
        }
        
        return new GroupShape(shapes.begin(), shapes.end());
    }

    virtual Gtk::Widget* property_widget()
    {
        return 0;
    }
    
    virtual void property_apply()
    {
    }

    virtual ShapeType* type() const;
private:
    void initialize();
    
    std::vector<Shape*> m_shapes;
};


} // namespace DBricks

#endif // GROUPSHAPE_HXX
