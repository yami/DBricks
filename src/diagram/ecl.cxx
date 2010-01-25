#include "ecl.hxx"
#include "SML.hxx"

#include <ecl/ecl.h>
#include <iostream>
#include <util/stl.hxx>

namespace EclSml {

void
EclSerializer::save_value(Sml::Value* value)
{
    m_stream<<indent()<<"(build-value :type \""<<data_type_to_string(value->type())<<"\" :value ";
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
    }
}
    
void
EclSerializer::save_attribute(Sml::Attribute* attribute)
{
    m_stream<<indent()<<"(build-attribute :name \""<<attribute->name()<<"\" :value\n";
    inc_indent();
    save_value(attribute->value());
    dec_indent();
    m_stream<<indent()<<")\n";
}

void
EclSerializer::save_object(Sml::Object* object)
{
    m_stream<<indent()<<"(build-object\n";
    inc_indent();
    for(Sml::Object::AttributesType::const_iterator iter = object->attributes().begin();
        iter != object->attributes().end();
        ++iter) {
        save_attribute((*iter).second);
    }
    dec_indent();
    m_stream<<indent()<<")\n";
}

void
EclSerializer::save_list(Sml::List* list)
{
    m_stream<<indent()<<"(build-list\n";
    inc_indent();
    for(Sml::List::ValuesType::const_iterator iter = list->values().begin();
        iter != list->values().end();
        ++iter) {
        save_value(*iter);
    }
    dec_indent();
    m_stream<<indent()<<")\n";
}

#define XCDR cl_cdr
#define XCAR cl_car


const char* loader_def =
                        "(progn"
                        "  (defun build-value (&key type value)\n"                        
                        "    (c-build-value type value))\n"
                        ""
                        "  (defun build-attribute (&key name value)\n"
                        "    (c-build-attribute name value))\n"
                        ""
                        "  (defun build-object (&rest attrs)\n"
                        "    (c-build-object attrs))\n"
                        ""
                        "  (defun build-list (&rest vals)\n"
                        "    (c-build-list vals)))\n";

const char* script =
                        "(build-value :type \"list\" :value \n"
                        "  (build-list\n"
                        "    (build-value :type \"object\" :value \n"
                        "      (build-object\n"
                        "        (build-attribute :name \"count\" :value\n"
                        "          (build-value :type \"int\" :value 1)\n"
                        "        )\n"
                        "        (build-attribute :name \"name\" :value \n"
                        "          (build-value :type \"string\" :value \"Ming\")\n"
                        "        )\n"
                        "      )\n"
                        "    )\n"
                        "    (build-value :type \"list\" :value \n"
                        "      (build-list\n"
                        "        (build-value :type \"int\" :value 1)\n"
                        "        (build-value :type \"string\" :value \"Shanghai\")\n"
                        "      )\n"
                        "    )\n"
                        "  ))\n";


cl_object evaluate(const char* str)
{
  return si_safe_eval(3, c_string_to_object(str), Cnil, OBJNULL);
}


std::string to_stl_string(cl_object str)
{
    assert(ecl_stringp(str));
    return std::string(ecl_base_string_pointer_safe(str));
}


cl_object c_build_value(cl_object type, cl_object value)
{
    Sml::DataType dtype = Sml::string_to_data_type(to_stl_string(type));
    Sml::Value* v = 0;
    
    switch (dtype) {
        case Sml::DT_Bool:
        {
            bool cvalue = fixint(value);
            v = new Sml::Value(cvalue);
        }
        break;

        case Sml::DT_Int:
        {
            int cvalue = fixint(value);
            v = new Sml::Value(cvalue);
        }
        break;

        case Sml::DT_Double:
        {
            double cvalue = ecl_to_double(value);
            v = new Sml::Value(cvalue);
        }
        break;

        case Sml::DT_String:
        {
            std::string cvalue = to_stl_string(value);
            v = new Sml::Value(cvalue);
        }
        break;

        case Sml::DT_Object:
        {
            Sml::Object* cvalue = (Sml::Object*)ecl_foreign_data_pointer_safe(value);
            v = new Sml::Value(cvalue);
        }
        break;

        case Sml::DT_List:
        {
            Sml::List* cvalue = (Sml::List*)ecl_foreign_data_pointer_safe(value);
            v = new Sml::Value(cvalue);
        }
        break;

        case Sml::DT_Unknown:
            assert(0);
            break;
    }

    return ecl_make_foreign_data(Cnil, 0, v);
}

cl_object c_build_attribute(cl_object name, cl_object value)
{
    std::string cname = to_stl_string(name);
    Sml::Value* cvalue = (Sml::Value*) ecl_foreign_data_pointer_safe(value);
    
    Sml::Attribute* attr = new Sml::Attribute(cname, cvalue);

    return ecl_make_foreign_data(Cnil, 0, attr);
}

cl_object c_build_object(cl_object attrs)
{
    Sml::Object* object = new Sml::Object();

    for (cl_object rest = attrs; rest != Cnil; rest = XCDR(rest)) {
        Sml::Attribute* attribute = (Sml::Attribute*) ecl_foreign_data_pointer_safe(XCAR(rest));
        object->add_attribute(attribute);
    }

    return ecl_make_foreign_data(Cnil, 0, object);
}

cl_object c_build_list(cl_object values)
{
    Sml::List* list = new Sml::List();

    for (cl_object rest = values; rest != Cnil; rest = XCDR(rest)) {
        Sml::Value* value = (Sml::Value*) ecl_foreign_data_pointer_safe(XCAR(rest));
        list->add_value(value);
    }

    return ecl_make_foreign_data(Cnil, 0, list);
}

void init(int argc, char** argv)
{
    cl_boot(argc, argv);
    
    cl_def_c_function(cl_intern(1, make_simple_base_string("C-BUILD-VALUE")),
                      (cl_objectfn_fixed)c_build_value, 2);    
    cl_def_c_function(cl_intern(1, make_simple_base_string("C-BUILD-ATTRIBUTE")),
                      (cl_objectfn_fixed)c_build_attribute, 2);
    cl_def_c_function(cl_intern(1, make_simple_base_string("C-BUILD-OBJECT")),
                      (cl_objectfn_fixed)c_build_object, 1);
    cl_def_c_function(cl_intern(1, make_simple_base_string("C-BUILD-LIST")),
                      (cl_objectfn_fixed)c_build_list, 1);

    evaluate(loader_def);
}

void fini ()
{
    cl_shutdown();
}

Sml::Object* load(const char* file)
{
    std::ifstream ifile(file);
    char* script = util::read_file(ifile);
    cl_object object = evaluate(script);
    delete[] script;
    
    return (Sml::Object*) ecl_foreign_data_pointer_safe(object);
}


} // namespace EclSml


