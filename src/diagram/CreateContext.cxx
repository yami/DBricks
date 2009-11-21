#include "CreateContext.hxx"

#include <logging/logging.hxx>

#include "defines.hxx"
#include "Diagram.hxx"
#include "CustomShape.hxx"
#include "RectShape.hxx"
#include "EllipseShape.hxx"

namespace DBricks {

bool
CreateContext::on_button_press_event(Shape* shape, GdkEventButton* e)
{
    static int s = 0;
    
    if (e->button == Left_Button) {
        if (s == 0) {
            DLOG(SHAPE, INFO, "CreateContext:: add rect shape\n");
            m_diagram->add_shape(new RectShape(10, 20, 40, 40));
            s = 1;
        } else {
            DLOG(SHAPE, INFO, "CreateContext:: add elli shape\n");
            m_diagram->add_shape(new EllipseShape(20, 40, 50, 50));
            s = 0;            
        }
    }
    return false;


    // o.k. we do not use this testing right now.
    if (0) {
        bool pass_down = false;

        if (e->button == Left_Button) {
            DLOG(DIAGRAM, DEBUG, "CreateContext: add shape seq.\n");
            m_diagram->add_shape(new CustomShape("seq"));
        } else {
            DLOG(DIAGRAM, DEBUG, "CreateContext: buttons other than Left_Button are not implemented yet!\n");
        }

        return pass_down;
    }    
}

} // namespace DBricks
