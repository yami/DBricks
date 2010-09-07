#include "snap.hxx"


static double grid_width;
static double grid_height;


namespace DBricks {

// TODO: a better design instead static variable


void snap_set_grid(double width, double height)
{
    grid_width  = width;
    grid_height = height;
}

Point snap(const Point& source)
{
    double x = round(source.x / grid_width) * grid_width;
    double y = round(source.y / grid_height) * grid_height;

    return Point(x, y);
}

}
