#ifndef SELECTION_HXX
#define SELECTION_HXX

#include <vector>
#include <util/stl.hxx>
#include <algorithm>

#include "Shape.hxx"
#include <geom/Rect.hxx>


namespace DBricks {

class Selection {
public:
    typedef std::vector<Shape*> ShapesType;    
    
    void select(Shape* shape)
    {
        ASSERT(!is_selected(shape));
        m_shapes.push_back(shape);
        shape->show_handles();
    }

    template<class ForwardIterT>
    void select(const Rect& bb, ForwardIterT begin, ForwardIterT end)
    {
        for (ForwardIterT iter = begin; iter != end; ++iter) {
            if ((*iter)->in(bb)) 
                select(*iter);
        }
    }
    
    bool is_selected(Shape* shape) const
    {
        return util::in_container(m_shapes, shape);
    }
    
    void unselect()
    {
        std::for_each(m_shapes.begin(), m_shapes.end(), std::mem_fun(&Shape::hide_handles));
        m_shapes.clear();
    }

    size_t size() const
    {
        return m_shapes.size();
    }

    bool empty() const
    {
        return m_shapes.empty();
    }
    
    Shape* operator [] (size_t i) const
    {
        ASSERT(i <= size());
        return m_shapes[i];
    }

    ShapesType& shapes()
    {
        return m_shapes;
    }
private:
    ShapesType m_shapes;
};


} // namespace DBricks


#endif // SELECTION_HXX
