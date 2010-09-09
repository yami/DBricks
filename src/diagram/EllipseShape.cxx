#include "EllipseShape.hxx"
#include <sml/Sml.hxx>
#include "IRenderer.hxx"
#include "DiagramArchiver.hxx"


namespace DBricks {

DEFINE_SHAPE_TYPE(EllipseShape, Ellipse_Shape_Type, "ellipse");

EllipseShape::EllipseShape(const Point& start, Handle*& handle)
    :BoxShape(start, handle)
{
}

void
EllipseShape::draw_shape(IRenderer* renderer) const
{
    // TODO: make line/fill spec set as properties
    renderer->line_width(1.0);
    renderer->line_color(Black);
    renderer->line_style(LS_Solid);
    renderer->fill_color(White);
    renderer->fill_alpha(1.0);    
    renderer->draw_ellipse(Point(m_x+m_width/2, m_y+m_height/2), m_width, m_height, Draw_Stroke | Draw_Fill);
}

void
EllipseShape::save(DiagramArchiver* ar) const
{
    Sml::Object* object = ar->object();
    
    object->add_attribute_data("name", "Ellipse");
    object->add_attribute_data("type", "Ellipse");

    object->add_attribute_data("x", m_x);
    object->add_attribute_data("y", m_y);
    object->add_attribute_data("width", m_width);
    object->add_attribute_data("height", m_height);
}

void
EllipseShape::load(DiagramArchiver* ar)
{
    Sml::Object* object = ar->object();
    
    object->get_attribute_data("x", m_x);
    object->get_attribute_data("y", m_y);
    object->get_attribute_data("width", m_width);
    object->get_attribute_data("height", m_height);

    initialize();
}


ShapeType* EllipseShape::type() const
{
    return Ellipse_Shape_Type;
}

} // namespace DBricks
