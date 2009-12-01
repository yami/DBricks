#ifndef BOXSHAPE_HXX
#define BOXSHAPE_HXX

#include "Shape.hxx"
#include "Handle.hxx"
#include "Connector.hxx"

namespace DBricks {

class BoxShape : public Shape {
public:
    BoxShape(double x, double y, double width, double height);
    BoxShape(const Rect& rect);

    
    virtual void move_handle(Handle* handle, const Point& delta);
    virtual void move_connector(Connector* connector, const Point& delta);
    virtual void move(const Point& delta);
    virtual bool cover (const Point& point) const;
    virtual bool in(const Rect& rect) const;
    virtual Rect bb() const;

protected:
    void update_handles();
    
    double& m_x;
    double& m_y;
    double  m_width;
    double  m_height;

    Handle  m_lhandle;
    Handle  m_rhandle;
    Handle  m_thandle;
    Handle  m_bhandle;

    Connector  m_lconnector;
    Connector  m_rconnector;
    Connector  m_tconnector;
    Connector  m_bconnector;    
};


} // namespace DBricks

#endif // BOXSHAPE_HXX
