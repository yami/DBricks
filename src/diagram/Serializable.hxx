#ifndef SERIALIZABLE_HPP
#define SERIALIZABLE_HPP

namespace DBricks {

class Archiver;

class Serializable {
public:
    virtual ~Serializable() {}
    virtual void serialize(Archiver* ar) const = 0;
};

} // namespace DBricks


#endif // SERIALIZABLE_HPP
