#include "CairoRenderer.hxx"
#include "Change.hxx"
#include <util/assert.hxx>
#include <logging/logging.hxx>

namespace DBricks {

// CairoRenderer::CairoRenderer(Cairo::RefPtr<Cairo::Context> ctx)
//     :m_ctx(ctx)
// {
//     m_line_spec.color = Black;
//     m_line_spec.width = 0.025;
//     m_line_spec.style = LS_Solid;
//     m_background      = White;
// }

CairoRenderer::CairoRenderer(Glib::RefPtr<Gdk::Window> window)
    :m_window(window)
{
    m_line_spec.color = Black;
    m_line_spec.width = 1.0;
    m_line_spec.style = LS_Solid;

    m_fill_spec.color = White;
    m_fill_spec.alpha = 1;
    
    m_background      = White;
}


void CairoRenderer::save()
{
    m_save_stack.push(std::vector<RendererModification*>());
}

void CairoRenderer::restore()
{
    std::vector<RendererModification*>& m = m_save_stack.top();

    int i = m.size();
    
    for (--i; i >= 0; --i) {
        switch (m[i]->type) {
            case MT_LineWidth:
                line_width__(*((double *)m[i]->data));
                break;
            case MT_LineColor:
                line_color__(*((Color *)m[i]->data));
                break;
            case MT_LineStyle:
                line_style__(*((LineStyle*)m[i]->data));
                break;
            case MT_BackgroundColor:
                background__(*((Color *)m[i]->data));
                break;
            case MT_FillColor:
                fill_color__(*((Color *)m[i]->data));
                break;
            case MT_FillAlpha:
                fill_alpha__(*((double *)m[i]->data));
                break;
        }

        delete m[i];
    }
    
    m_save_stack.pop();
}

void CairoRenderer::begin_render(const Rect& update)
{
    m_ctx = m_window->create_cairo_context();
}

void CairoRenderer::end_render()
{
}

void CairoRenderer::save_to_stack(ModificationType type, void* data)
{
    if (!m_save_stack.empty()) {
        m_save_stack.top().push_back(new RendererModification(type, data));
    }
}

Color CairoRenderer::background(const Color& color)
{
    save_to_stack(MT_BackgroundColor, &m_background);
    return background__(color);
}

Color CairoRenderer::background__(const Color& color)
{
    Color old_color = m_background;

    m_background = color;
    
    return old_color;
}

double CairoRenderer::line_width(double width)
{
    save_to_stack(MT_LineWidth, &m_line_spec.width);
    return line_width__(width);
}

double CairoRenderer::line_width__(double width)
{
    double old_width = m_line_spec.width;
    m_line_spec.width = width;
    m_ctx->set_line_width(width);

    return old_width;
}

Color CairoRenderer::line_color(const Color& color)
{
    save_to_stack(MT_LineColor, &m_line_spec.color);
    return line_color__(color);
}

Color CairoRenderer::line_color__(const Color& color)
{
    Color old_color = m_line_spec.color;
    m_line_spec.color = color;
    return old_color;
}

LineStyle CairoRenderer::line_style(LineStyle style)
{
    save_to_stack(MT_LineStyle, &m_line_spec.style);
    return line_style__(style);
}

LineStyle CairoRenderer::line_style__(LineStyle style)
{
    LineStyle old_style = m_line_spec.style;

    m_line_spec.style = style;

    std::vector<double> dash_vector;
        
    switch(style) {
        case LS_Solid:
            m_ctx->set_dash(dash_vector, 0);
            break;
        case LS_Dash:
            dash_vector.push_back(4);
            dash_vector.push_back(1);
            m_ctx->set_dash(dash_vector, 0);
            break;
        default:
            ASSERT_NOT_REACHED();
    }

    return old_style;
}

Color CairoRenderer::fill_color(const Color& color)
{
    save_to_stack(MT_FillColor, &m_fill_spec.color);
    return fill_color__(color);
}

Color CairoRenderer::fill_color__(const Color& color)
{
    Color old_color = m_fill_spec.color;
    m_fill_spec.color = color;
    return old_color;
}

double CairoRenderer::fill_alpha(double alpha)
{
    save_to_stack(MT_FillAlpha, &m_fill_spec.alpha);
    return fill_alpha__(alpha);
}

double CairoRenderer::fill_alpha__(double alpha)
{
    double old_alpha = m_fill_spec.alpha;
    m_fill_spec.alpha = alpha;
    return old_alpha;
}

void CairoRenderer::do_draw_action(int action)
{
    if (action & Draw_Fill) {
        m_ctx->set_source_rgba(RGB_P(m_fill_spec.color), m_fill_spec.alpha);
        m_ctx->fill_preserve();
    }
    
    if (action & Draw_Stroke) {
        m_ctx->set_source_rgba(RGB_P(m_line_spec.color), 1);
        m_ctx->stroke();
    }        
}


void CairoRenderer::draw_background(const Rect& update)
{
    m_ctx->set_source_rgba(RGB_P(m_background), 1);
    m_ctx->rectangle(update.x1(), update.y1(), update.width(), update.height());
    m_ctx->fill();
}

void CairoRenderer::draw_line(const Point& from, const Point& to)
{
    m_ctx->move_to(from.x, from.y);
    m_ctx->line_to(to.x, to.y);

    do_draw_action(Draw_Stroke);
}

void CairoRenderer::draw_rectangle(const Point& top_left, const Point& bottom_right, int action)
{
    m_ctx->rectangle(top_left.x, top_left.y, bottom_right.x - top_left.x, bottom_right.y - top_left.y);   
    do_draw_action(action);
}

void CairoRenderer::draw_ellipse(const Point& center, double width, double height, int action)
{
    m_ctx->save();
    
    m_ctx->translate(center.x, center.y);
    m_ctx->scale(width/2, height/2);
    m_ctx->arc(0, 0, 1, 0, 2*M_PI);    
    
    m_ctx->restore();

    do_draw_action(action);
}

void CairoRenderer::draw_polygon(const std::vector<Point>& points, int action)
{
    ASSERT(points.size() > 1);
    
    m_ctx->begin_new_path();
    m_ctx->move_to(points[0].x, points[0].y);

    for (size_t i = 1; i<points.size(); ++i) {
        m_ctx->line_to(points[i].x, points[i].y);
    }

    m_ctx->line_to(points[0].x, points[0].y);
    m_ctx->close_path();

    do_draw_action(action);
}

void CairoRenderer::draw_path(const std::vector<PathElement>& elements, int action)
{
    m_ctx->begin_new_path();
    
    for (size_t i = 0; i < elements.size(); ++i) {
        switch (elements[i].type) {
            case Path_Move_To:
                m_ctx->move_to(elements[i].points[0].x, elements[i].points[0].y);
                break;
            case Path_Line_To:
                m_ctx->line_to(elements[i].points[0].x, elements[i].points[0].y);
                break;
            case Path_Curve_To:
                m_ctx->curve_to(elements[i].points[0].x, elements[i].points[0].y,
                                elements[i].points[1].x, elements[i].points[1].y,
                                elements[i].points[2].x, elements[i].points[2].y);
        }
    }

    m_ctx->close_path();

    do_draw_action(action);
}

} // namespace DBricks
