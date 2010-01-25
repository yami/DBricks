#ifndef ECLSML_HXX
#define ECLSML_HXX

#include <ostream>
#include <string>
#include <cassert>


namespace Sml {
class Value;
class Attribute;
class Object;
class List;
}

namespace EclSml {

class EclSerializer {
public:
    EclSerializer(std::ostream& stream)
        :m_stream(stream), m_indent(""), m_depth(0)
    {
    }

    int inc_indent()
    {
        ++m_depth;
        m_indent += "  ";
        return m_depth - 1;
    }

    int dec_indent()
    {
        assert(m_depth > 0);
        m_indent = m_indent.substr(2);
        --m_depth;
        return m_depth + 1;
    }

    const std::string& indent() const
    {
        return m_indent;
    }
    
    void save_value(Sml::Value* value);
    
    void save_attribute(Sml::Attribute* attribute);

    void save_object(Sml::Object* object);

    void save_list(Sml::List* list);

private:    
    std::ostream& m_stream;
    std::string   m_indent;
    int           m_depth;
};

Sml::Object* load(const char* file);

void init(int argc, char** argv);
void fini ();


} // namespace EclSml


#endif  // ECLSML_HXX
