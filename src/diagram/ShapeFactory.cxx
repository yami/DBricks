#include "ShapeFactory.hxx"

#include "EllipseShape.hxx"
#include "LineShape.hxx"
#include "RectShape.hxx"
#include "GroupShape.hxx"

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

} // namespace DBricks
