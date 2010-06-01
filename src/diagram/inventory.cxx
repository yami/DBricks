#include "Shape.hxx"

namespace DBricks {

extern ShapeTypeInventory theInventory;

extern ShapeType* Line_Shape_Type;
extern ShapeType* Rect_Shape_Type;
extern ShapeType* Ellipse_Shape_Type;

extern ShapeType* read_shape(const std::string& filename);

ShapeTypeCollection* standard_collection()
{
    ShapeTypeCollection* collection = new ShapeTypeCollection("standard");

    collection->add(Line_Shape_Type);
    collection->add(Rect_Shape_Type);
    collection->add(Ellipse_Shape_Type);

    return collection;
}

ShapeTypeCollection* boxed_collection()
{
    ShapeTypeCollection* collection = new ShapeTypeCollection("boxed");

    collection->add(read_shape("/home/yami/project/DBricks/src/shapes/flowchart/predefined_process.shape"));
    collection->add(read_shape("/home/yami/project/DBricks/src/shapes/flowchart/merge.shape"));
    return collection;
}

void inventory_init()
{
    theInventory.add_collection(standard_collection());
    theInventory.add_collection(boxed_collection());
}

} // namespace DBricks
