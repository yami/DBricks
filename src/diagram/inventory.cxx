#include "Shape.hxx"

namespace DBricks {

extern ShapeTypeInventory theInventory;

extern ShapeType* Line_Shape_Type;
extern ShapeType* Rect_Shape_Type;
extern ShapeType* Ellipse_Shape_Type;

ShapeTypeCollection* standard_collection()
{
    ShapeTypeCollection* collection = new ShapeTypeCollection("standard");

    collection->add(Line_Shape_Type);
    collection->add(Rect_Shape_Type);
    collection->add(Ellipse_Shape_Type);

    return collection;
}

void inventory_init()
{
    theInventory.add_collection(standard_collection());
}

} // namespace DBricks
