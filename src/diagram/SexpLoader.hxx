#ifndef SEXPLOADER_HXX
#define SEXPLOADER_HXX

#include <sml/ISerializer.hxx>
#include <iostream>

namespace DBricks {

class SexpLoader: public Sml::ILoader {
public:
    SexpLoader(std::istream& stream)
        :m_stream(stream)
    {
    }
    
    virtual Sml::Object* load();
private:
    std::istream& m_stream;
};

Sml::Object* sexp_file_to_sml(const char* filename);

} // namespace DBricks

#endif // SEXPLOADER_HXX
