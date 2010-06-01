#ifndef STRING_HXX
#define STRING_HXX

#include <cstring>

namespace util {

inline bool string_equal(const char* s1, const char* s2)
{
    using namespace std;
    return !strcmp(s1, s2);
}


inline bool string_iequal(const char* s1, const char* s2)
{
    using namespace std;
    return !strcasecmp(s1, s2);
}

} // namespace 

#endif // STRING_HXX
