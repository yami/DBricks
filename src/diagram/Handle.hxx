#ifndef HANDLE_HXX
#define HANDLE_HXX

#include <string>
#include <cmath>
#include <geom/Point.hxx>

namespace DBricks {

class Handle {
public:
    Handle(const std::string& name, const Point& point)
        :m_name(name), m_point(point)
    {
    }

    ~Handle()
    {
    }
    
    const std::string& name() const
    {
        return m_name;
    }

    const Point& point() const
    {
        return m_point;
    }

    void point(const Point& point)
    {
        m_point = point;
    }
    
    double distance(const Point& point) const
    {
        return std::fabs(m_point.x - point.x) + std::fabs(m_point.y - point.y);
    }
    
private:
    std::string m_name;
    Point m_point;
};

} // namespace diagram

#endif  // HANDLE_HXX
