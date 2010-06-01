#include "RectShape.hxx"
#include <sml/Sml.hxx>
#include "IRenderer.hxx"

namespace DBricks {

DEFINE_SHAPE_TYPE(RectShape, Rect_Shape_Type, "rectangle");

RectShape::RectShape(const Point& start, Handle*& handle)
    :BoxShape(start, handle)
{
}

void
RectShape::draw_shape(IRenderer* renderer) const
{
    renderer->draw_rectangle(Point(m_x, m_y), Point(m_x+m_width, m_y+m_height), Fill_Fill);
}

void
RectShape::save(Sml::Object* object) const
{
    object->add_attribute_data("name", "Rectangle");
    object->add_attribute_data("type", "Rectangle");

    object->add_attribute_data("x", m_x);
    object->add_attribute_data("y", m_y);
    object->add_attribute_data("width", m_width);
    object->add_attribute_data("height", m_height);
}

void
RectShape::load(Sml::Object* object)
{
    object->get_attribute_data("x", m_x);
    object->get_attribute_data("y", m_y);
    object->get_attribute_data("width", m_width);
    object->get_attribute_data("height", m_height);

    initialize();
}

ShapeType* RectShape::type() const
{
    return Rect_Shape_Type;
}

} // namespace DBricks
