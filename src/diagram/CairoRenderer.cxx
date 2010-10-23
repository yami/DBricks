#include "CairoRenderer.hxx"
#include "Change.hxx"
#include <util/assert.hxx>
#include <logging/logging.hxx>

#include <pango/pangocairo.h>

namespace DBricks {


CairoRenderer::CairoRenderer(Glib::RefPtr<Gdk::Window> window)
    :m_window(window), m_layout(0)
{
    m_line_spec.color = Black;
    m_line_spec.width = 1.0;
    m_line_spec.style = LS_Solid;

    m_fill_spec.color = White;
    m_fill_spec.alpha = 1;
    
    m_background      = White;
}

CairoRenderer::~CairoRenderer()
{
    if (m_layout)
        g_object_unref(m_layout);
}


void CairoRenderer::begin_render(const Rect& update)
{
    m_ctx = m_window->create_cairo_context();

    if (m_layout)
        g_object_unref(m_layout);
    
    m_layout = pango_cairo_create_layout(m_ctx->cobj());
}

void CairoRenderer::end_render()
{
}


void CairoRenderer::set_line_spec()
{
    m_ctx->set_line_width(m_line_spec.width);
    m_ctx->set_source_rgba(RGB_P(m_line_spec.color), 1);

    std::vector<double> dash_vector;
    switch (m_line_spec.style) {
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
}

void CairoRenderer::set_fill_spec()
{
    m_ctx->set_source_rgba(RGB_P(m_fill_spec.color), m_fill_spec.alpha);
}

void CairoRenderer::do_draw_action(int action)
{
    if (action & Draw_Fill) {
        set_fill_spec();
        m_ctx->fill_preserve();
    }
    
    if (action & Draw_Stroke) {
        set_line_spec();
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
    set_line_spec();
    
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

static PangoStyle
FontStyle_to_PangoStyle(FontStyle style)
{
    switch (style) {
        case FontStyle_Normal:
            return PANGO_STYLE_NORMAL;
        case FontStyle_Oblique:
            return PANGO_STYLE_OBLIQUE;
        case FontStyle_Italic:
            return PANGO_STYLE_ITALIC;
    }

    return PANGO_STYLE_NORMAL;
}

static PangoVariant
FontVariant_to_PangoVariant(FontVariant variant)
{
    switch (variant) {
        case FontVariant_Normal:
            return PANGO_VARIANT_NORMAL;
        case FontVariant_SmallCaps:
            return PANGO_VARIANT_SMALL_CAPS;
    }

    return PANGO_VARIANT_NORMAL;
}

static PangoWeight
FontWeight_to_PangoWeight(int weight)
{
    return static_cast<PangoWeight>(weight);
}

static PangoStretch
FontStretch_to_PangoStretch(FontStretch stretch)
{
    switch (stretch) {
        case FontStretch_Ultra_Condensed:
            return PANGO_STRETCH_ULTRA_CONDENSED;
        case FontStretch_Extra_Condensed:
            return PANGO_STRETCH_EXTRA_CONDENSED;
        case FontStretch_Condensed:
            return PANGO_STRETCH_CONDENSED;
        case FontStretch_Semi_Condensed:
            return PANGO_STRETCH_SEMI_CONDENSED;
        case FontStretch_Normal:
            return PANGO_STRETCH_NORMAL;
        case FontStretch_Semi_Expanded:
            return PANGO_STRETCH_SEMI_EXPANDED;
        case FontStretch_Expanded:
            return PANGO_STRETCH_EXPANDED;
        case FontStretch_Extra_Expanded:
            return PANGO_STRETCH_EXTRA_EXPANDED;
        case FontStretch_Ultra_Expanded:
            return PANGO_STRETCH_ULTRA_EXPANDED;
    }

    return PANGO_STRETCH_NORMAL;
}

static gint
FontSize_to_PangoSize(int size)
{
    return size;
}

static PangoFontDescription*
FontDesc_to_PangoFontDescription(const FontDesc& fdesc)
{
    PangoFontDescription* pfd = pango_font_description_new();

    if (fdesc.mask & FontMask_Family) {
        pango_font_description_set_family(pfd, fdesc.family);        
    }

    if (fdesc.mask & FontMask_Style) {
        pango_font_description_set_style(pfd, FontStyle_to_PangoStyle(fdesc.style));
    }

    if (fdesc.mask & FontMask_Variant) {
        pango_font_description_set_variant(pfd, FontVariant_to_PangoVariant(fdesc.variant));
    }

    if (fdesc.mask & FontMask_Weight) {
        pango_font_description_set_weight(pfd, FontWeight_to_PangoWeight(fdesc.weight));
    }

    if (fdesc.mask & FontMask_Stretch) {
        pango_font_description_set_stretch(pfd, FontStretch_to_PangoStretch(fdesc.stretch));
    }

    if (fdesc.mask & FontMask_Size) {
        pango_font_description_set_size(pfd, FontSize_to_PangoSize(fdesc.size));
    }
    
    return pfd;
}


void CairoRenderer::draw_text(const TextString& text, int action)
{
    m_ctx->move_to(40, 60);
    
    for (std::list<TextAttributes>::const_iterator iattr = text.attrs.begin();
         iattr != text.attrs.end();
         ++iattr) {
        PangoFontDescription *fdesc = FontDesc_to_PangoFontDescription(iattr->font);
        pango_layout_set_font_description(m_layout, fdesc);
        pango_font_description_free(fdesc);

        std::string substr = text.text.substr(iattr->start, iattr->end - iattr->start);
        pango_layout_set_text(m_layout, substr.c_str(), substr.size());

        pango_cairo_update_context(m_ctx->cobj(), pango_layout_get_context(m_layout));
        pango_layout_context_changed(m_layout);
        
        pango_cairo_layout_path(m_ctx->cobj(), m_layout);

        if (action & Draw_Fill) {
            m_ctx->set_source_rgba(RGB_P(iattr->fill_color), 1);
            m_ctx->fill_preserve();
        }

        if (action & Draw_Stroke) {
            m_ctx->set_line_width(1);
            m_ctx->set_source_rgba(RGB_P(iattr->outline_color), 1);
            m_ctx->stroke();
        }
    }
}

} // namespace DBricks
