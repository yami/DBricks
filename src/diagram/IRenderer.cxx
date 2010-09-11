#include "IRenderer.hxx"

namespace DBricks {


void IRenderer::background(const Color& color)
{
    m_background = color;
}

void IRenderer::line_width(double width)
{
    m_line_spec.width = width;
}

void IRenderer::line_color(const Color& color)
{
    m_line_spec.color = color;
}

void IRenderer::line_style(LineStyle style)
{
    m_line_spec.style = style;
}

void IRenderer::fill_color(const Color& color)
{
    m_fill_spec.color = color;
}

void IRenderer::fill_alpha(double alpha)
{
    m_fill_spec.alpha = alpha;
}

} // namespace DBricks
