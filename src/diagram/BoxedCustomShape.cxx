#include "BoxedCustomShape.hxx"
#include <sml/Sml.hxx>

#include <logging/logging.hxx>

namespace DBricks {

BoxedCustomShape::BoxedCustomShape(const Point& start, Handle*& handle)
    :BoxShape(start, handle), m_template("ell.xml")
{
    handle = &m_sehandle;
    initialize();
}

BoxedCustomShape::BoxedCustomShape(const Rect& rect)
    :BoxShape(rect), m_template("ell.xml")
{
    initialize();
}

BoxedCustomShape::BoxedCustomShape(double x, double y, double width, double height)
    :BoxShape(x, y, width, height), m_template("ell.xml")
{
    initialize();
}


BoxedCustomShape::BoxedCustomShape()
    :BoxShape(), m_template("ell.xml")
{
}

void
BoxedCustomShape::draw_shape(Cairo::RefPtr<Cairo::Context> ctx) const
{
    GError* gerror;    
    RsvgHandle* handle = rsvg_handle_new_from_data((const guint8*)m_shape_string.c_str(), strlen(m_shape_string.c_str()), &gerror);
    
    ctx->save();
    ctx->translate(m_corner.x, m_corner.y);
    rsvg_handle_render_cairo(handle, ctx->cobj());
    rsvg_handle_free(handle);
    ctx->restore();
}

void
BoxedCustomShape::save(Sml::Object* object) const
{
    object->add_attribute_data("name", "BoxedCustomShape");
    object->add_attribute_data("type", "BoxedCustomShape");

    object->add_attribute_data("x", m_x);
    object->add_attribute_data("y", m_y);
    object->add_attribute_data("width", m_width);
    object->add_attribute_data("height", m_height);
}

void
BoxedCustomShape::load(Sml::Object* object)
{
    object->get_attribute_data("x", m_x);
    object->get_attribute_data("y", m_y);
    object->get_attribute_data("width", m_width);
    object->get_attribute_data("height", m_height);

    initialize();
}


void
BoxedCustomShape::on_data_change()
{
    m_template.bind("width", m_width);
    m_template.bind("height", m_height);
    m_shape_string = m_template.fill_in();
}

} // namespace DBricks
