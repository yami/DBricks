#ifndef ISMLSERIALIZER_HXX
#define ISMLSERIALIZER_HXX

namespace Sml {

class Value;
class Attribute;
class Object;
class List;

class ISaver {
public:
    virtual ~ISaver() {}
    virtual void save(Object* object) = 0;
};

class ILoader {
public:
    virtual ~ILoader() {}
    virtual Object* load() = 0;
};

} // namespace sml

#endif // ISMLSERIALIZER_HXX
