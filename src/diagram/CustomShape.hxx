#ifndef CUSTOMSHAPE_HXX
#define CUSTOMSHAPE_HXX

#include <string>

#include <util/assert.hxx>
#include <geom/Point.hxx>

#include "Shape.hxx"

namespace DBricks {

class CustomShape : public Shape {
public:
    CustomShape(const std::string& name);
    
    ~CustomShape()
    {
    }    

    virtual void move_handle(Handle* handle, const Point& delta);
    virtual void move_connector(Connector* connector, const Point& delta)
    {
    }
    virtual void move(const Point& delta);
    virtual bool cover(const Point& point) const
    {
        return false;
    }

    virtual bool in(const Rect& rect) const
    {
        ASSERT_NOT_IMPLEMENTED();
        return false;
    }

    virtual Rect bb() const
    {
        ASSERT_NOT_IMPLEMENTED();
        return Rect(0, 0, 0, 0);
    }

    virtual void save (Sml::Object* object) const {}
    virtual void load (Sml::Object* object) {}
private:
    virtual void draw_shape(Cairo::RefPtr<Cairo::Context> ctx) const;
    
    void update_handle(const std::string& name, const Point& point);
    void update_corner(const Point& delta);
    void read_handles();

    std::string m_name;
};

} // namespace diagram

#endif // CUSTOMSHAPE_HXX
