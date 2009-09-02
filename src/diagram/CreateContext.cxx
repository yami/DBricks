#include "CreateContext.hxx"

#include <logging/logging.hxx>

#include "defines.hxx"
#include "Diagram.hxx"
#include "CustomShape.hxx"


namespace DBricks {

bool
CreateContext::on_button_press_event(GdkEventButton* e)
{
    bool pass_down = false;

    if (e->button == Left_Button) {
        DLOG(DIAGRAM, DEBUG, "CreateContext: add shape seq.\n");
        m_diagram->add_shape(new CustomShape("seq"));
    } else {
        DLOG(DIAGRAM, DEBUG, "CreateContext: buttons other than Left_Button are not implemented yet!\n");
    }

    return pass_down;
}

} // namespace DBricks
