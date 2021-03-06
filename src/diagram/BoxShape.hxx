#ifndef BOXSHAPE_HXX
#define BOXSHAPE_HXX

#include "Shape.hxx"
#include "Handle.hxx"
#include "Connector.hxx"

namespace DBricks {

class BoxShape : public Shape {
public:
    typedef void (BoxShape::*MenuActionMethodType) ();

    BoxShape();
    BoxShape(const Point& start, Handle*& handle);
    BoxShape(double x, double y, double width, double height);
    BoxShape(const Rect& rect);
    
    static void initialize_class();
    
    virtual void move_handle(Handle* handle, const Point& delta);
    virtual void move(const Point& delta);
    virtual bool cover (const Point& point) const;
    virtual bool in(const Rect& rect) const;
    virtual Rect bb() const;
    virtual Menu* menu(const Point& point) const;
    
    void set_fixed_aspect();
    void set_square();

    virtual Gtk::Widget* property_widget();
    virtual void         property_apply();
protected:
    void update_handles();
    void initialize();

    virtual void on_data_change()
    {
    }
    
    static Menu s_menu;
    
    double& m_x;
    double& m_y;
    double  m_width;
    double  m_height;

    Connector  m_lconnector;
    Connector  m_rconnector;
    Connector  m_tconnector;
    Connector  m_bconnector;
    Connector  m_nwconnector;
    Connector  m_neconnector;
    Connector  m_seconnector;
    Connector  m_swconnector;

    
    Handle  m_lhandle;
    Handle  m_rhandle;
    Handle  m_thandle;
    Handle  m_bhandle;
    Handle  m_nwhandle;
    Handle  m_nehandle;
    Handle  m_sehandle;
    Handle  m_swhandle;
};


} // namespace DBricks

#endif // BOXSHAPE_HXX
