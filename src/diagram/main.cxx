#include <gtkmm/main.h>

#include <logging/logging.hxx>

#include "Desktop.hxx"
#include "BoxShape.hxx"

int main(int argc, char** argv)
{
    DLOG_SET_LEVEL(DEBUG);
    
    Gtk::Main kit(argc, argv);

    // TODO: move to else where
    DBricks::BoxShape::initialize();

    
    DBricks::Desktop desktop;
    desktop.show_all();

    Gtk::Main::run(desktop);
}
