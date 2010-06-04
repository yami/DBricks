#include "SexpSaver.hxx"

#include <util/assert.hxx>
#include <sml/Sml.hxx>
#include <fstream>

using namespace std;

namespace DBricks {

void SexpSaver::save(Sml::Object* object)
{
    ios_base::fmtflags old_flags = m_stream.flags();
    m_stream.precision(6);
    m_stream.setf(ios::fixed);

    save_object(object);
    
    m_stream.flags(old_flags);
}

void SexpSaver::save_value(Sml::Value* value)
{
    m_stream<<indent()<<"(value \""<<data_type_to_string(value->type())<<"\" ";
    switch (value->type()) {
        case Sml::DT_Bool:
            m_stream<<value->get_bool()<<")\n";
            break;
        case Sml::DT_Int:
            m_stream<<value->get_int()<<")\n";
            break;
        case Sml::DT_Double:
            m_stream<<value->get_double()<<")\n";
            break;
        case Sml::DT_String:
            m_stream<<"\""<<value->get_string()<<"\")\n";
            break;
        case Sml::DT_Object:
            m_stream<<"\n";
            inc_indent();
            save_object(value->get_object());
            dec_indent();
            m_stream<<indent()<<")\n";
            break;
        case Sml::DT_List:
            m_stream<<"\n";
            inc_indent();
            save_list(value->get_list());
            dec_indent();
            m_stream<<indent()<<")\n";
            break;
        case Sml::DT_Pointer:
            m_stream<<value->get_pointer()<<")\n";
            break;
        default:
            ASSERT_NOT_REACHED();
    }
}

void SexpSaver::save_attribute(Sml::Attribute* attribute)
{
    m_stream<<indent()<<"(attribute \""<<attribute->name()<<"\" \n";
    inc_indent();
    save_value(attribute->value());
    dec_indent();
    m_stream<<indent()<<")\n";    
}

void SexpSaver::save_object(Sml::Object* object)
{
    m_stream<<indent()<<"(object\n";
    inc_indent();
    for(Sml::Object::AttributesType::const_iterator iter = object->attributes().begin();
        iter != object->attributes().end();
        ++iter) {
        save_attribute((*iter).second);
    }
    dec_indent();
    m_stream<<indent()<<")\n";    
}

void SexpSaver::save_list(Sml::List* list)
{
    m_stream<<indent()<<"(list\n";
    inc_indent();
    for(Sml::List::ValuesType::const_iterator iter = list->values().begin();
        iter != list->values().end();
        ++iter) {
        save_value(*iter);
    }
    dec_indent();
    m_stream<<indent()<<")\n";    
}

int SexpSaver::inc_indent()
{
    ++m_depth;
    m_indent += "  ";
    return m_depth - 1;
}

int SexpSaver::dec_indent()
{
    assert(m_depth > 0);
    m_indent = m_indent.substr(2);
    --m_depth;
    return m_depth + 1;    
}


void sml_to_sexp_file(const char* filename, Sml::Object* object)
{
    std::ofstream ofile(filename);
    SexpSaver saver(ofile);
    saver.save(object);
}

} // namespace DBricks
