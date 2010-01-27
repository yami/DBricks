#include "RectShape.hxx"
#include "SML.hxx"

namespace DBricks {

RectShape::RectShape(const Point& start, Handle*& handle)
    :BoxShape(start, handle)
{
}

void
RectShape::draw_shape(Cairo::RefPtr<Cairo::Context> ctx) const
{
    ctx->save();
    ctx->rectangle(m_x, m_y, m_width, m_height);

    ctx->set_source_rgba(1, 1, 1, 1); // set to white
    ctx->fill_preserve();

    ctx->set_source_rgba(0.117, 0.337, 0.612, 1); // set to blue
    ctx->stroke();
    
    ctx->restore();
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

} // namespace DBricks
