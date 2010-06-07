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


static const std::string Slash = "/";

void collection_files_init(const std::string& collection_name, const std::string& dirname)
{
    ShapeTypeCollection* collection = new ShapeTypeCollection(collection_name);
    Glib::Dir dir(dirname);
    std::string dirent_name;

    while ((dirent_name = dir.read_name()) != "") {
        std::string dirent_fullname = dirname + Slash + dirent_name;
        collection->add(read_shape(dirent_fullname));
    }

    theInventory.add_collection(collection);
}

void shape_files_init(const std::string& shape_dirname)
{
    Glib::Dir   dir(shape_dirname);
    std::string dirent_name;

    while ((dirent_name = dir.read_name()) != "") {
        std::string dirent_fullname = shape_dirname + Slash + dirent_name;

        if (Glib::file_test(dirent_fullname, Glib::FILE_TEST_EXISTS | Glib::FILE_TEST_IS_DIR)) {
            collection_files_init(dirent_name, dirent_fullname);            
        }
    }
}

void inventory_init()
{
    theInventory.add_collection(standard_collection());

    shape_files_init("/home/yami/project/DBricks/src/shapes");
}

} // namespace DBricks
