#include <gtkmm/main.h>

#include <logging/logging.hxx>

#include "Desktop.hxx"
#include "BoxShape.hxx"
#include "ModifyContext.hxx"


#include <ecl/ecl.h>
#include "ecl.hxx"

int main(int argc, char** argv)
{
    DLOG_SET_LEVEL(DEBUG);

    Gtk::Main kit(argc, argv);

    // TODO: move to else where
    DBricks::BoxShape::initialize_class();
    DBricks::ModifyContext::initialize_class();
    
    DBricks::Desktop desktop;
    desktop.show_all();

    Gtk::Main::run(desktop);
    
    cl_shutdown();
}
