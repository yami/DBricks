#include "ShapeFactory.hxx"

#include "EllipseShape.hxx"
#include "LineShape.hxx"
#include "RectShape.hxx"
#include "GroupShape.hxx"
#include "BoxedCustomShape.hxx"


namespace DBricks {

Shape* ShapeFactory::create_shape(const std::string& type)
{
    if (type == "Rectangle")
        return new RectShape();

    if (type == "Ellipse")
        return new EllipseShape();

    if (type == "Line")
        return new LineShape();

    if (type == "Group")
        return new GroupShape();

    ASSERT_NOT_REACHED();
}

Shape* ShapeFactory::create_shape(const std::string& type, const Point& start, Handle*& handle)
{
    if (type == "Rectangle")
        return new RectShape(start, handle);

    if (type == "Ellipse")
        return new EllipseShape(start, handle);

    if (type == "Line")
        return new LineShape(start, handle);

    if (type == "Group")
        return new GroupShape(start, handle);

    if (type == "Custom")
        return new BoxedCustomShape(start, handle);
    
    ASSERT_NOT_REACHED();
}



} // namespace DBricks
