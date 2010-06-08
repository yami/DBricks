#include "Handle.hxx"
#include "Shape.hxx"
#include "IRenderer.hxx"

namespace DBricks {

void
Handle::draw(IRenderer* renderer) const
{
    double hw = 2.0;
    
    renderer->save();
    renderer->fill_color(Blue);
    renderer->line_color(Green);
    renderer->draw_rectangle(Point(m_point.x-hw, m_point.y-hw), Point(m_point.x+hw, m_point.y+hw), Draw_Stroke | Draw_Fill);
    renderer->restore();
}

} // namespace DBricks
