#include "BoxedShape.hxx"
#include "IRenderer.hxx"
#include <sml/Sml.hxx>
#include <ssexp/ssexp.h>
#include <util/string.hxx>
#include <util/stl.hxx>
#include "preference.hxx"
#include <logging/logging.hxx>

#include "PropertyMap.hxx"
#include "PropertyDescriptor.hxx"
#include "IntProperties.hxx"
#include "DiagramArchiver.hxx"

#include <algorithm>
using std::max;

using namespace ssexp;
using util::string_iequal;

namespace DBricks {

Point
evaluate_point(sexp_t pair)
{
    return Point(ci_to_double(first(pair)), ci_to_double(second(pair)));
}

PathElement
evaluate_path_element(sexp_t element)
{
    PathElement e;
    const char* command = ci_symbol_name(car(element));

    if (string_iequal(command, "move-to")) {
        e.type = Path_Move_To;
        e.points[0] = evaluate_point(second(element));
        return e;
    }

    if (string_iequal(command, "line-to")) {
        e.type = Path_Line_To;
        e.points[0] = evaluate_point(second(element));
        return e;
    }

    if (string_iequal(command, "curve-to")) {
        e.type = Path_Curve_To;
        e.points[0] = evaluate_point(ci_nth(element, 1));
        e.points[1] = evaluate_point(ci_nth(element, 2));
        e.points[2] = evaluate_point(ci_nth(element, 3));
        return e;
    }

    ASSERT_NOT_REACHED();
}

DrawInst*
evaluate_inst(sexp_t inst)
{
    const char* inst_op = ci_symbol_name(car(inst));

    if (string_iequal(inst_op, "rectangle")) {
        Point topleft = evaluate_point(ci_nth(inst, 1));
        double width  = ci_to_double(ci_nth(inst, 2));
        double height = ci_to_double(ci_nth(inst ,3));

        return new DrawInstRectangle(topleft, width, height);
    }

    if (string_iequal(inst_op, "ellipse")) {
        Point  center = evaluate_point(ci_nth(inst, 1));
        double width  = ci_to_double(ci_nth(inst, 2));
        double height = ci_to_double(ci_nth(inst ,3));

        return new DrawInstEllipse(center, width, height);        
    }

    if (string_iequal(inst_op, "line")) {
        Point from = evaluate_point(ci_nth(inst, 1));
        Point to   = evaluate_point(ci_nth(inst, 2));

        return new DrawInstLine(from, to);
    }

    if (string_iequal(inst_op, "polygon")) {
        std::vector<Point> points;
        for (sexp_t rest_points = cdr(inst); rest_points != Nil; rest_points = cdr(rest_points)) {
            points.push_back(evaluate_point(car(rest_points)));
        }

        return new DrawInstPolygon(points);
    }

    if (string_iequal(inst_op, "path")) {
        std::vector<PathElement> elements;
        for (sexp_t rest_elements = cdr(inst); rest_elements != Nil; rest_elements = cdr(rest_elements)) {
            elements.push_back(evaluate_path_element(car(rest_elements)));
        }

        return new DrawInstPath(elements);
    }

    assert(0);
    return 0;
}

BoxedShapeInfo*
evaluate_shape_definition(sexp_t shape_def)
{
    if (!string_iequal(ci_symbol_name(car(shape_def)), "shape"))
        return 0;

    BoxedShapeInfo* info = new BoxedShapeInfo();

    for (sexp_t rest = cdr(shape_def); rest != Nil; rest = cdr(rest)) {
        sexp_t elem = car(rest);
        const char* field = ci_symbol_name(car(elem));
        if (string_iequal(field, "name")) {
            info->name = strdup(ci_to_string(second(elem)));
        } else if (string_iequal(field, "description")) {
            info->description = strdup(ci_to_string(second(elem)));
        } else if (string_iequal(field, "width")) {
            info->width = ci_to_double(second(elem));
        } else if (string_iequal(field, "height")) {
            info->height = ci_to_double(second(elem));
        } else if (string_iequal(field, "connectors")) {
            sexp_t connectors = cdr(elem);
            
            info->nconnector  = ci_length(connectors);
            info->connectors  = new Point[info->nconnector];
            
            for (int iconn=0; iconn < info->nconnector; ++iconn, connectors = cdr(connectors)) {
                sexp_t p = car(connectors);
                info->connectors[iconn].x = ci_to_double(first(p));
                info->connectors[iconn].y = ci_to_double(second(p));                
            }
        } else if (string_iequal(field, "draw")) {
            for (sexp_t instructions = cdr(elem); instructions != Nil; instructions = cdr(instructions)) {
                info->inst_list.push_back(evaluate_inst(car(instructions)));
            }
        } else {
            assert(0);
        }
    }

    return info;
}

// TODO: remove this
ShapeTypeCollection Boxed_Collection("boxed");

ShapeType*
read_shape(const std::string& filename)
{
    char* content = util::read_file(filename);
    sexp_t shape_def = ssexp::read_from_string(content);
    BoxedShapeInfo* info = evaluate_shape_definition(shape_def);
    
    info->type = new BoxedShapeType(info);

    return info->type;
}

void
DrawInstLine::draw(IRenderer* renderer, const LinearTransform& trans) const
{
    renderer->draw_line(trans(from), trans(to));
}

void
DrawInstRectangle::draw(IRenderer* renderer, const LinearTransform& trans) const
{    
    renderer->draw_rectangle(trans(topleft),
                             trans(Point(topleft.x + width, topleft.y + height)),
                             Draw_Stroke);
}

void
DrawInstEllipse::draw(IRenderer* renderer, const LinearTransform& trans) const
{
    renderer->draw_ellipse(trans(center), trans.x(width), trans.y(height), Draw_Stroke);
}

void
DrawInstPolygon::draw(IRenderer* renderer, const LinearTransform& trans) const
{
    std::vector<Point> points;

    for (size_t i=0; i<m_points.size(); ++i) {
        points.push_back(trans(m_points[i]));
    }

    renderer->draw_polygon(points, Draw_Stroke);
}

void
DrawInstPath::draw(IRenderer* renderer, const LinearTransform& trans) const
{
    std::vector<PathElement> elements(m_elements);

    for (size_t i=0; i<elements.size(); ++i) {
        switch (elements[i].type) {
            case Path_Move_To:
            case Path_Line_To:
                elements[i].points[0] = trans(elements[i].points[0]);
                break;
            case Path_Curve_To:
                elements[i].points[0] = trans(elements[i].points[0]);
                elements[i].points[1] = trans(elements[i].points[1]);
                elements[i].points[2] = trans(elements[i].points[2]);
                break;
        }
    }

    renderer->draw_path(elements, Draw_Stroke);
}

BoxedShape::BoxedShape(BoxedShapeInfo* info)
    :m_info(info)
{
}


BoxedShape::BoxedShape(BoxedShapeInfo* info, const Point& start, Handle*& handle)
    :m_x(start.x), m_y(start.y), m_info(info)
{
    m_width  = Default_Width;
    m_height = Default_Width * info->height / info->width;
    
    initialize();    
    update_data();

    handle = m_handles[HDir_SE];
}

void
BoxedShape::initialize()
{
    for (int iconn = 0; iconn < m_info->nconnector; ++iconn)
        m_connectors.push_back(new Connector(this));

    for (int ihandle = 0; ihandle < HDir_Max; ++ihandle)
        m_handles.push_back(new Handle(this));
}

void
BoxedShape::update_data()
{
    m_handles[HDir_N ]->point(Point(m_x + m_width/2, m_y));
    m_handles[HDir_NE]->point(Point(m_x + m_width,   m_y));
    m_handles[HDir_E ]->point(Point(m_x + m_width,   m_y + m_height/2));
    m_handles[HDir_SE]->point(Point(m_x + m_width,   m_y + m_height));
    m_handles[HDir_S ]->point(Point(m_x + m_width/2, m_y + m_height));
    m_handles[HDir_SW]->point(Point(m_x,             m_y + m_height));
    m_handles[HDir_W ]->point(Point(m_x,             m_y + m_height/2));
    m_handles[HDir_NW]->point(Point(m_x,             m_y));

    LinearTransform trans(m_width/m_info->width, m_height/m_info->height, m_x, m_y);

    int iconn = 0;
    for (ConnectorsType::iterator iter = m_connectors.begin(); iter != m_connectors.end(); ++iter, ++iconn) {
        (*iter)->point(trans(m_info->connectors[iconn]));
    }
}

void
BoxedShape::draw_shape(IRenderer* renderer) const
{
    LinearTransform trans(m_width/m_info->width, m_height/m_info->height, m_x, m_y);
    
    for (std::list<DrawInst*>::iterator iter = m_info->inst_list.begin();
         iter != m_info->inst_list.end();
         ++iter) {
        // TODO: add set line/fill spec instructions.
        renderer->line_width(1.0);
        renderer->line_color(Black);
        renderer->line_style(LS_Solid);
        renderer->fill_color(White);
        renderer->fill_alpha(1.0);
        (*iter)->draw(renderer, trans);
    }
}

void
BoxedShape::move_handle(Handle* handle, const Point& delta)
{
#define CALC_N do {                                                     \
        double new_height  = max(Min_Height, m_height - delta.y);       \
        m_y               -= new_height - m_height;                     \
        m_height           = new_height;                                \
    } while (0)

#define CALC_E m_width = max(Min_Width, m_width + delta.x)

#define CALC_S m_height = max(Min_Height, m_height + delta.y)
    
#define CALC_W do {                             \
        double new_width  = max(Min_Width, m_width - delta.x);   \
        m_x              -= new_width - m_width;                 \
        m_width           = new_width;                           \
    } while (0)


    const double Min_Width  = 5;
    const double Min_Height = 5;
    
    if (handle == m_handles[HDir_N]) {
        CALC_N;
    } else if (handle == m_handles[HDir_NE]) {
        CALC_E;
        CALC_N;
    } else if (handle == m_handles[HDir_E]) {
        CALC_E;
    } else if (handle == m_handles[HDir_SE]) {
        CALC_S;
        CALC_E;
    } else if (handle == m_handles[HDir_S]) {
        CALC_S;
    } else if (handle == m_handles[HDir_SW]) {
        CALC_W;
        CALC_S;
    } else if (handle == m_handles[HDir_W]) {
        CALC_W;
    } else if (handle == m_handles[HDir_NW]) {
        CALC_W;
        CALC_N;
    } else {
        ASSERT_NOT_REACHED();
    }
    
    update_data();
}

void
BoxedShape::move(const Point& delta)
{
    m_x += delta.x;
    m_y += delta.y;

    update_data();
}

bool
BoxedShape::cover(const Point& point) const
{
    bool covered = point.x >= m_x && point.x <= m_x + m_width &&
        point.y >= m_y && point.y <= m_y + m_height;
    
    return covered;
}

bool
BoxedShape::in(const Rect& rect) const
{
    return
        m_x >= rect.x1() && m_y >= rect.y1() &&
        (m_x + m_width) <= rect.x2() && (m_y + m_height) <= rect.y2();
}

Rect
BoxedShape::bb() const
{
    return Rect(Point(m_x, m_y), m_width, m_height);
}


void
BoxedShape::save(DiagramArchiver* ar) const
{
    Sml::Object* object = ar->object();
    
    object->add_attribute_data("x", m_x);
    object->add_attribute_data("y", m_y);
    object->add_attribute_data("width", m_width);
    object->add_attribute_data("height", m_height);
}

void
BoxedShape::load(DiagramArchiver* ar)
{
    Sml::Object* object = ar->object();
    
    object->get_attribute_data("x", m_x);
    object->get_attribute_data("y", m_y);
    object->get_attribute_data("width", m_width);
    object->get_attribute_data("height", m_height);
    
    initialize();
    update_data();
}

BoxedShape* BoxedShape::clone() const
{
    BoxedShape* copy = new BoxedShape(m_info);

    init_copy(copy);

    copy->m_x      = m_x;
    copy->m_y      = m_y;
    copy->m_width  = m_width;
    copy->m_height = m_height;

    return copy;
}

ShapeType* BoxedShape::type() const
{
    return m_info->type;
}


static const char* Boxed_Property_Layout =
    "(table :rows 2 :cols 2"
    "    (property \"X\" x)         (property \"Y\" y)"
    "    (property \"Width\" width) (property \"Height\" height)"
    ")";


Gtk::Widget*
BoxedShape::property_widget()
{
    if (!m_property_descriptor) {
        m_property_map = new PropertyMap(this);

        m_property_map->add(new NumberProperty<double>("X", m_x, 0, 400));
        m_property_map->add(new NumberProperty<double>("Y", m_y, 0, 400));
        m_property_map->add(new NumberProperty<double>("WIDTH", m_width, 0, 200));
        m_property_map->add(new NumberProperty<double>("HEIGHT", m_height, 0, 200));

        m_property_descriptor = new PropertyDescriptor(*m_property_map, Boxed_Property_Layout);
    }

    return m_property_descriptor->to_widget();    
}

void
BoxedShape::property_apply()
{
    update_data();
}

} // namespace DBricks
