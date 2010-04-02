#ifndef SEXPSAVER_HXX
#define SEXPSAVER_HXX

#include <sml/ISerializer.hxx>
#include <iostream>

namespace DBricks {

class SexpSaver: public Sml::ISaver {
public:
    SexpSaver(std::ostream& stream)
        :m_stream(stream), m_indent(""), m_depth(0)
    {
    }

    virtual void save(Sml::Object* object);
private:    
    void save_value(Sml::Value* value);
    void save_attribute(Sml::Attribute* attribute);
    void save_object(Sml::Object* object);
    void save_list(Sml::List* list);

    int inc_indent();
    int dec_indent();

    const std::string& indent() const
    {
        return m_indent;
    }

    std::ostream& m_stream;
    std::string   m_indent;
    int           m_depth;
};

void sml_to_sexp_file(const char* filename, Sml::Object* object);

} // namespace DBricks

#endif // SEXPSAVER_HXX
