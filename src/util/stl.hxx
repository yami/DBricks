#ifndef STL_HXX
#define STL_HXX

#include <algorithm>

namespace util {

template<typename ForwardIterT>
void delete_entries(ForwardIterT begin, ForwardIterT end)
{
    for (ForwardIterT iter=begin; iter != end; ++iter) {
        delete *iter;
    }
}

template<typename ContainerT>
void delete_container(ContainerT* c)
{
    delete_entries(c->begin(), c->end());
    delete c;
}

template<class ContainerT, class ValueT>
void delete_value(ContainerT& c, const ValueT& v)
{
    c.erase(std::remove(c.begin(), c.end(), v), c.end());
}

template<class ContainerT, class PredicatorT>
void delete_if(ContainerT& c, PredicatorT p)
{
    c.erase(std::remove_if(c.begin(), c.end(), p), c.end());
}

template<class ContainerT, class ValueT>
bool in_container(const ContainerT& c, const ValueT& v)
{
    return std::find(c.begin(), c.end(), v) != c.end();
}

} // namespace util

#endif //STL_HXX
