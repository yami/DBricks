#ifndef BUILTINCHANGES_HXX
#define BUILTINCHANGES_HXX

#include <vector>
#include "Change.hxx"

namespace DBricks {

class Handle;
class Shape;

typedef std::vector<Shape*> ShapeList;

class ShapeMoveChange: public Change {
public:
    ShapeMoveChange(Diagram* diagram, const ShapeList& shapes, const Point& original_point, const Point& current_point)
        :m_diagram(diagram), m_shapes(shapes.begin(), shapes.end()), m_opoint(original_point), m_cpoint(current_point)
    {
    }
    
    virtual void apply()
    {
        Diagram::move_shapes(m_shapes, m_cpoint - m_opoint);
    }
    
    virtual void revert()
    {
        Diagram::move_shapes(m_shapes, m_opoint - m_cpoint);
    }

private:
    Diagram*  m_diagram;
    ShapeList m_shapes;
    Point     m_opoint;
    Point     m_cpoint;
};

class HandleMoveChange: public Change {
public:
    HandleMoveChange(Diagram* diagram, Handle* handle, const Point& original_point, const Point& current_point)
        :m_diagram(diagram), m_handle(handle), m_opoint(original_point), m_cpoint(current_point)
    {
    }
    
    virtual void apply()
    {
        Diagram::move_handle(m_handle->shape(), m_handle, m_cpoint - m_opoint);
    }
    
    virtual void revert()
    {
        Diagram::move_handle(m_handle->shape(), m_handle, m_opoint - m_cpoint);
    }

private:
    Diagram*  m_diagram;
    Handle*   m_handle;
    Point     m_opoint;
    Point     m_cpoint;
};


class ShapeCreateChange: public Change {
public:
    ShapeCreateChange(Diagram* diagram, Shape* shape)
        :m_diagram(diagram), m_shape(shape)
    {
    }

    virtual void apply()
    {
        m_diagram->add_shape(m_shape);
    }

    virtual void revert()
    {
        m_diagram->del_shape(m_shape);
    }

private:
    Diagram* m_diagram;
    Shape*   m_shape;
};


} // namespace DBricks


#endif // BUILTINCHANGES_HXX
