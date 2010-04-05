#ifndef BOXEDCUSTOMSHAPE_HXX
#define BOXEDCUSTOMSHAPE_HXX

#include "BoxShape.hxx"
#include "IRenderer.hxx"

#include "TextTemplate.hxx"

#include <librsvg/rsvg.h>
#include <librsvg/rsvg-cairo.h>


namespace DBricks {

class BoxedCustomShape: public BoxShape {
public:
    BoxedCustomShape();
    BoxedCustomShape(const Point& start, Handle*& handle);
    BoxedCustomShape(double x, double y, double width, double height);
    BoxedCustomShape(const Rect& rect);

    virtual void save (Sml::Object* object) const;
    virtual void load (Sml::Object* object);    

    virtual BoxedCustomShape* clone() const
    {
        return new BoxedCustomShape(m_x, m_y, m_width, m_height);
    }
private:
    virtual void draw_shape(IRenderer* renderer) const;
    virtual void on_data_change();

    std::string m_shape_string;
    TextTemplate m_template;
};


} // namespace DBricks

#endif // BOXEDCUSTOMSHAPE_HXX
