#include <gtkmm/main.h>

#include <logging/logging.hxx>

#include "Display.hxx"
#include "Diagram.hxx"


int main(int argc, char** argv)
{
    DLOG_SET_LEVEL(DEBUG);
    
    Gtk::Main kit(argc, argv);

    DBricks::Diagram diagram;
    DBricks::Display display(&diagram);

    diagram.attach_observer(&display);
    
    display.show_all();

    Gtk::Main::run(display);
}
