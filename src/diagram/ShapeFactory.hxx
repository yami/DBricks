#ifndef SHAPEFACTORY_HXX
#define SHAPEFACTORY_HXX

#include <string>

namespace DBricks {

class Shape;

class ShapeFactory {
public:
    static Shape* create_shape(const std::string& shape_type);
};

} // namespace DBricks


#endif // SHAPEFACTORY_HXX
