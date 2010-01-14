#ifndef ARCHIVER_HXX
#define ARCHIVER_HXX

namespace DBricks {

class Archiver {
public:
    virtual void object_begin(const std::string& name) = 0;
    virtual void object_end(const std::string& name) = 0;

    virtual void list_begin(const std::string& name) = 0;
    virtual void list_end(const std::string& name) = 0;
    
    virtual void serialize(const std::string& name, double value) = 0;
    virtual void serialize(const std::string& name, int    value) = 0;
};

} // namespace DBricks


#endif // ARCHIVER_HXX
