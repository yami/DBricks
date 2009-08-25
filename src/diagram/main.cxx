#include "Display.hxx"
#include "Diagram.hxx"


#include <gtkmm/main.h>

int main(int argc, char** argv)
{
    Gtk::Main kit(argc, argv);

    DBricks::Diagram diagram;
    DBricks::Display display(&diagram);

    diagram.attach_observer(&display);
    
    display.show_all();

    Gtk::Main::run(display);
}
