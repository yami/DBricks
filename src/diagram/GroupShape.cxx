#include "GroupShape.hxx"
#include <algorithm>

#include <sml/Sml.hxx>

#include "ShapeFactory.hxx"
#include "IRenderer.hxx"
#include "DiagramArchiver.hxx"

namespace DBricks {

ShapeTypeAbstract<GroupShape>  Group_Shape_Type_Object("group");
ShapeTypeAbstract<GroupShape>* Group_Shape_Type = &Group_Shape_Type_Object;


void
GroupShape::draw_shape(IRenderer* renderer) const
{
    for (std::vector<Shape*>::const_iterator iter = m_shapes.begin();
         iter != m_shapes.end();
         ++iter) {
        (*iter)->draw(renderer);
    }
    
    if (m_show_handles) {
        Rect rect = bb();
        renderer->draw_rectangle(Point(rect.x1(), rect.y1()), Point(rect.x2(), rect.y2()));
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
GroupShape::save(DiagramArchiver* ar) const
{
    Sml::Object* object = ar->object();
    
    Sml::List* list = new Sml::List();
    object->add_attribute_data("items", list);

    for(std::vector<Shape*>::const_iterator iter = m_shapes.begin();
        iter != m_shapes.end();
        ++iter) {
        Sml::Object* shape_object = new Sml::Object();
        list->add_value(new Sml::Value(shape_object));

        ar->save_shape(shape_object, *iter);
    }
}

void
GroupShape::load(DiagramArchiver* ar)
{
    Sml::Object* object = ar->object();
    
    Sml::List* list;
    object->get_attribute_data("items", list);
    for (Sml::List::ValuesType::const_iterator iter = list->values().begin();
         iter != list->values().end();
         ++iter) {
        Sml::Object* shape_object = (*iter)->get_object();
        m_shapes.push_back(ar->load_shape(shape_object));
    }

    initialize();
    BoxShape::initialize();
}

void
GroupShape::initialize()
{
    Rect r = m_shapes[0]->bb();

    double x1 = r.x1();
    double y1 = r.y1();
    double x2 = r.x2();
    double y2 = r.y2();

    for (size_t i=1; i<m_shapes.size(); ++i) {
        Rect rect = m_shapes[i]->bb();

        x1 = std::min(x1, rect.x1());
        y1 = std::min(y1, rect.y1());
        x2 = std::max(x2, rect.x2());
        y2 = std::max(y2, rect.y2());
    }

    m_x = x1;
    m_y = y1;
    m_width  = x2 - x1;
    m_height = y2 - y1;    
}

ShapeType* GroupShape::type() const
{
    return Group_Shape_Type;
}

} // namespace DBricks
