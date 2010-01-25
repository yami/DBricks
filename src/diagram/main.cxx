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

    EclSml::init(argc, argv);
    
    Gtk::Main kit(argc, argv);

    // TODO: move to else where
    DBricks::BoxShape::initialize();
    DBricks::ModifyContext::initialize();
    
    DBricks::Desktop desktop;
    desktop.show_all();

    Gtk::Main::run(desktop);

    EclSml::fini();
    
    cl_shutdown();
}
