#ifndef CUSTOMSHAPE_HXX
#define CUSTOMSHAPE_HXX

#include <string>

#include <geom/Point.hxx>
#include "Shape.hxx"

namespace DBricks {

class CustomShape : public Shape {
    CustomShape(const std::string& name);
    
    ~CustomShape()
    {
    }
    
    virtual void draw (Cairo::RefPtr<Cairo::Context> ctx) const;
    virtual void move_handle(Handle* handle, const Point& delta);
    virtual void move(const Point& delta);
    virtual double distance(const Point& point) const;
    
private:
    void update_handle(const std::string& name, const Point& point);
    void update_corner(const Point& delta);
    void read_handles();

    std::string m_name;
};

} // namespace diagram

#endif // CUSTOMSHAPE_HXX
