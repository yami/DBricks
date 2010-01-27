#ifndef SHAPEFACTORY_HXX
#define SHAPEFACTORY_HXX

#include <string>

namespace DBricks {

class Shape;
class Handle;
class Point;

class ShapeFactory {
public:
    static Shape* create_shape(const std::string& shape_type);
    static Shape* create_shape(const std::string& type, const Point& start, Handle*& handle);
};

} // namespace DBricks


#endif // SHAPEFACTORY_HXX
