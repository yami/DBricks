#ifndef STL_HXX
#define STL_HXX

#include <algorithm>
#include <iostream>
#include <fstream>

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

inline
std::streampos file_size(std::ifstream& ifile)
{
    std::streampos curr = ifile.tellg();
    std::streampos size = ifile.seekg(0, std::ios::end).tellg();
    ifile.seekg(curr, std::ios::beg);

    return size;
}

inline
char* read_file(std::ifstream& ifile)
{
    std::streampos size = file_size(ifile);
    char* buffer = new char[(size_t)size+1];
    ifile.read(buffer, size);
    return buffer;
}

inline
char* read_file(const std::string& filename)
{
    std::ifstream input(filename.c_str());
    return read_file(input);
}

inline
std::streampos stream_size(std::istream& stream)
{
    std::streampos curr = stream.tellg();
    std::streampos size = stream.seekg(0, std::ios::end).tellg();
    stream.seekg(curr, std::ios::beg);

    return size;
}

inline
char* read_stream(std::istream& stream)
{
    std::streampos size = stream_size(stream);
    char* buffer = new char[(size_t)size+1];
    stream.read(buffer, size);
    return buffer;
}

} // namespace util

#endif //STL_HXX
