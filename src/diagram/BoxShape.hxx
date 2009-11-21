#ifndef BOXSHAPE_HXX
#define BOXSHAPE_HXX

#include "Shape.hxx"
#include "Handle.hxx"

namespace DBricks {

class BoxShape : public Shape {
public:
    BoxShape(double x, double y, double width, double height);
    
    virtual void move_handle(Handle* handle, const Point& delta);
    virtual void move(const Point& delta);
    virtual bool cover (const Point& point) const;
    virtual bool in(const Rect& rect) const;
private:
    void update_handles();
    
protected:    
    double& m_x;
    double& m_y;
    double  m_width;
    double  m_height;

    Handle  m_lhandle;
    Handle  m_rhandle;
    Handle  m_thandle;
    Handle  m_bhandle;
};


} // namespace DBricks

#endif // BOXSHAPE_HXX
