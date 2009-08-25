#ifndef SHAPE_HXX
#define SHAPE_HXX

#include <vector>
#include <geom/Point.hxx>
#include <cairomm/context.h>

namespace DBricks {

class Handle;

class Shape {
public:
    typedef std::vector<Handle*> HandlesType;

    Shape()
        :m_corner(0, 0)
    {
    }

    ~Shape()
    {
    }

    virtual void draw (Cairo::RefPtr<Cairo::Context> ctx) const = 0;
    virtual void move_handle(Handle* handle, const Point& delta) = 0;
    virtual void move(const Point& delta) = 0;
    virtual double distance(const Point& point) const = 0;

    HandlesType& handles()
    {
        return m_handles;
    }

    const Point& corner() const
    {
        return m_corner;
    }
protected:
    HandlesType m_handles;
    Point m_corner;
};

} // namespace DBricks

#endif // SHAPE_HXX
