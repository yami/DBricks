#include "CustomShape.hxx"

#include <iostream>
#include <fstream>

#include <glibmm/spawn.h>
#include <cairomm/context.h>
#include <librsvg/rsvg.h>
#include <librsvg/rsvg-cairo.h>

#include "Handle.hxx"

namespace DBricks {


CustomShape::CustomShape(const std::string& name)
    :m_name(name)
{
    std::string command = "./custom_gen.pl " + name + " default";
    Glib::spawn_command_line_sync(command.c_str());
    read_handles();
}

void
CustomShape::draw_shape(Cairo::RefPtr<Cairo::Context> ctx) const
{
    GError* gerr;
    std::string filename = m_name + ".tmpl.out";
    RsvgHandle* handle = rsvg_handle_new_from_file(filename.c_str(), &gerr);
    if (!handle) {
        printf("Error when rsvg_handle_new_from_file\n");
        return;
    }

    ctx->save();
    ctx->translate(m_corner.x, m_corner.y);
    rsvg_handle_render_cairo(handle, ctx->cobj());
    rsvg_handle_free(handle);
    ctx->restore();
}

void
CustomShape::move_handle(Handle* handle, const Point& delta)
{
    char* command;
    std::string out;
    std::string err;
    int status;

    Point corner_delta;
        
    asprintf(&command, "./custom_gen.pl %s update %s %g %g", m_name.c_str(), handle->name().c_str(), delta.x, delta.y);
    Glib::spawn_command_line_sync(command, &out, &err, &status);
    free(command);
        
    if (!WIFEXITED(status) || WEXITSTATUS(status)) {
        std::cout<<"error while executing custom_gen.pl update"<<std::endl;
        return;
    }

    sscanf(out.c_str(), "%lf %lf", &corner_delta.x, &corner_delta.y);
        
    update_corner(corner_delta);
    read_handles();

}

void
CustomShape::move(const Point& delta)
{
    update_corner(delta);
}

void
CustomShape::update_handle(const std::string& name, const Point& point)
{
    for (HandlesType::iterator iter=m_handles.begin();
         iter != m_handles.end();
         ++iter) {
        if ((*iter)->name() == name) {
            (*iter)->point(point);
            return;
        }
    }

    m_handles.push_back(new Handle(name, this, point));
}

void
CustomShape::update_corner(const Point& delta)
{
    m_corner += delta;
}

void
CustomShape::read_handles()
{
    std::string filename = m_name + ".hndl.out";
    std::ifstream infile(filename.c_str());
    std::string line;
        
    while (std::getline(infile, line)) {
        if (line.find_first_not_of(" \t\r\n") == std::string::npos)
            continue;
            
        char name[1024];
        double x, y;

        sscanf(line.c_str(), "%s %lf %lf", &name[0], &x, &y);
        update_handle(name, Point(x, y));
    }
}

} // namespace diagram
