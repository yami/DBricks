#include "SexpLoader.hxx"
#include <ssexp/ssexp.h>
#include <sml/Sml.hxx>
#include <util/string.hxx>
#include <util/assert.hxx>
#include <util/stl.hxx>

#include <fstream>

using namespace ssexp;
using util::string_equal;

namespace DBricks {

Sml::Value* sexp_to_value(sexp_t value);
Sml::Attribute* sexp_to_attribute(sexp_t attribute);
Sml::Object* sexp_to_object(sexp_t object);
Sml::List* sexp_to_list(sexp_t list);

Sml::Value* sexp_to_value(sexp_t value)
{
    assert(string_equal(ci_symbol_name(car(value)), "VALUE"));

    const char* type = ci_to_string(second(value));
    sexp_t      v    = third(value);
    
    if (string_equal(type, "bool")) {
        bool sml_value = ci_to_integer(v);
        return new Sml::Value(sml_value);
    }

    if (string_equal(type, "int")) {
        int sml_value = ci_to_integer(v);
        return new Sml::Value(sml_value);
    }

    if (string_equal(type, "double")) {
        double sml_value = ci_to_double(v);
        return new Sml::Value(sml_value);
    }

    if (string_equal(type, "string")) {
        std::string sml_value = ci_to_string(v);
        return new Sml::Value(sml_value);
    }

    if (string_equal(type, "object")) {
        Sml::Object* sml_value = sexp_to_object(v);
        return new Sml::Value(sml_value);
    }

    if (string_equal(type, "list")) {
        Sml::List* sml_value = sexp_to_list(v);
        return new Sml::Value(sml_value);
    }

    if (string_equal(type, "pointer")) {
        void* sml_value = (void *)ci_to_integer(v);
        return new Sml::Value(sml_value);
    }
    
    ASSERT_NOT_REACHED();

    return 0;
}

Sml::Attribute* sexp_to_attribute(sexp_t attribute)
{
    assert(string_equal(ci_symbol_name(car(attribute)), "ATTRIBUTE"));

    const char* name  = ci_to_string(second(attribute));
    Sml::Value* value = sexp_to_value(third(attribute));

    return new Sml::Attribute(name, value);
}

Sml::Object* sexp_to_object(sexp_t object)
{
    assert(string_equal(ci_symbol_name(car(object)), "OBJECT"));

    Sml::Object* sml_object = new Sml::Object();
    
    for (sexp_t rest = cdr(object); rest != Nil; rest = cdr(rest)) {
        Sml::Attribute* attribute = sexp_to_attribute(car(rest));
        sml_object->add_attribute(attribute);
    }

    return sml_object;
}

Sml::List* sexp_to_list(sexp_t list)
{
    assert(string_equal(ci_symbol_name(car(list)), "LIST"));

    Sml::List* sml_list = new Sml::List();
    
    for (sexp_t rest = cdr(list); rest != Nil; rest = cdr(rest)) {
        Sml::Value* value = sexp_to_value(car(rest));
        sml_list->add_value(value);
    }

    return sml_list;
}

Sml::Object* SexpLoader::load()
{
    char* sexp_string = util::read_stream(m_stream);

    sexp_t sexp = ssexp::read_from_string(sexp_string);
    Sml::Object* object = sexp_to_object(sexp);
    
    delete[] sexp_string;

    return object;
}

Sml::Object* sexp_file_to_sml(const char* filename)
{
    std::ifstream stream(filename);
    SexpLoader loader(stream);
    return loader.load();
}

} // namespace DBricks
