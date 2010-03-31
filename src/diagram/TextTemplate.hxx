#ifndef TEXTTEMPLATE_HXX
#define TEXTTEMPLATE_HXX

#include <map>
#include <string>
#include <sstream>
#include <util/ecl.hxx>
#include <util/assert.hxx>
#include <logging/logging.hxx>

#include <iostream>
using namespace std;

namespace DBricks {

class TextTemplate {
public:    
    TextTemplate(const std::string& filename)
    {
        if (!loaded) {
            ecl::load("cl-text-template.lisp");
            loaded = true;
        }
        
        m_compile_string = "(compile-template-from-file \"" + filename + "\")";
    }

    ~TextTemplate()
    {
        ASSERT_NOT_REACHED();
    }
    
    void bind(const std::string& name, double value)
    {
        m_name_values[name] = value;
    }

    std::string fill_in()
    {
        cl_object bindings = ecl::evaluate(bind_string().c_str());

        cl_object compiled_template = ecl::evaluate(m_compile_string.c_str());
        return ecl::to_stl_string(fill_in_template(compiled_template, bindings));
    }
private:
    std::string bind_string()
    {
        std::stringstream ss;
        ss<<"'(";
        for (std::map<std::string, double>::iterator iter = m_name_values.begin();
             iter != m_name_values.end();
             ++iter) {
            ss<<"("<<iter->first<<" . "<<iter->second<<") ";
        }
        ss<<")";

        return ss.str();
    }
    
    cl_object fill_in_template(cl_object compiled_template, cl_object bindings)
    {
        return cl_funcall(3, cl_intern(1, make_simple_base_string("FILL-IN-TEMPLATE")), compiled_template, bindings);
    }

    
    static bool loaded;
    static int  count;
    
    std::map<std::string, double> m_name_values;
    std::string m_compile_string;
};

} // namespace DBricks


#endif // TEXTTEMPLATE_HXX
