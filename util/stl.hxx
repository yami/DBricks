#ifndef STL_HXX
#define STL_HXX

namespace util {

template<typename ForwardIter>
void delete_entries(ForwardIter begin, ForwardIter end)
{
    for (ForwardIter iter=begin; iter != end; ++iter) {
        delete *iter;
    }
}

template<typename Container>
void delete_container(Container* c)
{
    delete_entries(c->begin(), c->end());
    delete c;
}

} // namespace util

#endif //STL_HXX
