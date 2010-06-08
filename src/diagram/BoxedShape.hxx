#include <geom/Point.hxx>
#include <geom/Rect.hxx>
#include <list>

#include "Shape.hxx"
#include "IRenderer.hxx"

namespace Sml {
class Object;
};

namespace DBricks {

class LinearTransform {
public:
    LinearTransform(double xscale, double yscale, double x, double y)
        :m_xscale(xscale), m_yscale(yscale), m_x(x), m_y(y)
    {}

    Point operator () (const Point& point) const
    {
        double x = point.x * m_xscale + m_x;
        double y = point.y * m_yscale + m_y;

        return Point(x, y);
    }

    Rect operator () (const Rect& rect) const
    {
        double x1     = rect.x1() * m_xscale + m_x;
        double y1     = rect.y1() * m_yscale + m_y;
        double width  = rect.width() * m_xscale;
        double height = rect.height() * m_yscale;

        return Rect(Point(x1, y1), width, height);
    }

    double x(double c) const
    {
        return c * m_xscale;
    }

    double y(double c) const
    {
        return c * m_yscale;
    }
    
    const double m_xscale;
    const double m_yscale;
    const double m_x;
    const double m_y;
};

enum DrawInstOp {
    DIO_Line,
    DIO_Rectangle,
    DIO_Ellipse,
    DIO_Polygon,
    DIO_Path,
};

struct DrawInst {
    DrawInst(DrawInstOp inst_op)
        :op(inst_op)
    {}

    virtual void draw(IRenderer* renderer, const LinearTransform& trans) const = 0;
    
    DrawInstOp op;
};

struct DrawInstLine : public DrawInst {
    DrawInstLine(const Point& f, const Point& t)
        :DrawInst(DIO_Line), from(f), to(t)
    {}

    virtual void draw(IRenderer* renderer, const LinearTransform& trans) const;
    
    Point from;
    Point to;
};

struct DrawInstRectangle : public DrawInst {
    DrawInstRectangle(const Point& tl, double w, double h)
        :DrawInst(DIO_Rectangle), topleft(tl), width(w), height(h)
    {}

    virtual void draw(IRenderer* renderer, const LinearTransform& trans) const;
    
    Point topleft;
    double width;
    double height;
};

struct DrawInstEllipse : public DrawInst {
    DrawInstEllipse(const Point& c, double w, double h)
        :DrawInst(DIO_Ellipse), center(c), width(w), height(h)
    {}

    virtual void draw(IRenderer* renderer, const LinearTransform& trans) const;
    
    Point center;
    double width;
    double height;
};


struct DrawInstPolygon : public DrawInst {
    DrawInstPolygon(const std::vector<Point>& points)
        :DrawInst(DIO_Polygon), m_points(points)
    {
    }

    virtual void draw(IRenderer* renderer, const LinearTransform& trans) const;

    std::vector<Point> m_points;
};

struct DrawInstPath : public DrawInst {
    DrawInstPath(const std::vector<PathElement>& elements)
        :DrawInst(DIO_Path), m_elements(elements)
    {
    }

    virtual void draw(IRenderer* renderer, const LinearTransform& trans) const;
    
    std::vector<PathElement> m_elements;
};

struct BoxedShapeInfo {
    // read from shape file
    char* name;
    char* description;
    
    char* filename;

    int    nconnector;
    Point* connectors;

    double width;
    double height;

    std::list<DrawInst*> inst_list;

    // not read from shape file
    ShapeType* type;
};

class BoxedShape: public Shape {
public:
    BoxedShape(BoxedShapeInfo* info);
    BoxedShape(BoxedShapeInfo* info, const Point& start, Handle*& handle);

    virtual void draw_shape(IRenderer* renderer) const;
    virtual void move_handle(Handle* handle, const Point& delta);
    virtual void move(const Point& delta);
    virtual bool cover(const Point& point) const;
    virtual bool in(const Rect& rect) const;
    virtual Rect bb() const;
    
    virtual void save(DiagramArchiver* ar) const;
    virtual void load(DiagramArchiver* ar);

    virtual Gtk::Widget* property_widget();
    
    virtual void property_apply();

    virtual BoxedShape* clone() const;
    virtual ShapeType* type() const;
private:
    void initialize();
    void update_data();
    
    enum HandleDir {
        HDir_N,
        HDir_NE,
        HDir_E,
        HDir_SE,
        HDir_S,
        HDir_SW,
        HDir_W,
        HDir_NW,
        HDir_Max
    };

    double m_x;
    double m_y;
    double m_width;
    double m_height;

    BoxedShapeInfo* m_info;
};

class BoxedShapeType : public ShapeType {
public:
    BoxedShapeType(BoxedShapeInfo* info)
        :ShapeType(info->name), m_info(info)
    {}

    virtual BoxedShape* create(const Point& start, Handle*& handle)
    {
        return new BoxedShape(m_info, start, handle);
    }
    
    virtual BoxedShape* create()
    {
        return new BoxedShape(m_info);
    }
private:
    BoxedShapeInfo* m_info;
};

} // namespace DBricks
