#include "ShapeFactory.hxx"

#include "EllipseShape.hxx"
#include "LineShape.hxx"
#include "RectShape.hxx"
#include "GroupShape.hxx"


namespace DBricks {

Shape* ShapeFactory::create_shape(const std::string& type)
{
    return lookup_shape_type(type)->create();
}

Shape* ShapeFactory::create_shape(const std::string& type, const Point& start, Handle*& handle)
{
    return lookup_shape_type(type)->create(start, handle);
}

} // namespace DBricks
