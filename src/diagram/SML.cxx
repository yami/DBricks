#include "SML.hxx"

namespace Sml {

std::string data_type_to_string(DataType type)
{
    switch(type) {
        case DT_Unknown:
            return "unknown";
        case DT_Bool:
            return "bool";
        case DT_Int:
            return "int";            
        case DT_Double:
            return "double";
        case DT_String:
            return "string";
        case DT_Object:
            return "object";
        case DT_List:
            return "list";
    }
}

DataType   string_to_data_type(const std::string& ctype)
{
    if (ctype == "bool")
        return DT_Bool;

    if (ctype == "int")
        return DT_Int;

    if (ctype == "double")
        return DT_Double;

    if (ctype == "string")
        return DT_String;

    if (ctype == "object")
        return DT_Object;

    if (ctype == "list")
        return DT_List;

    return DT_Unknown;
}

} // namespace Sml
