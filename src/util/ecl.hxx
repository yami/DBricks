#include <ecl/ecl.h>
#include <string>
#include <cassert>

#define XCDR    cl_cdr
#define XCAR    cl_car
#define XFIRST  cl_first
#define XSECOND cl_second
#define XTHIRD  cl_third


namespace ecl {

inline cl_object
evaluate(const char* str)
{
  return si_safe_eval(3, c_string_to_object(str), Cnil, OBJNULL);
}


inline std::string
to_stl_string(cl_object str)
{
    assert(ecl_stringp(str));
    return std::string(ecl_base_string_pointer_safe(si_copy_to_simple_base_string(str)));
}

inline void
load(const char* filename)
{
    cl_load(1, make_base_string_copy(filename));
}


inline cl_object
string_to_object(const char* str)
{
    return c_string_to_object(str);
}

inline cl_object
string_to_object(const std::string& str)
{
    return c_string_to_object(str.c_str());
}

} // namespace ecl