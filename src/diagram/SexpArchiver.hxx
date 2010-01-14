#ifndef SEXPARCHIVER_HPP
#define SEXPARCHIVER_HPP

#include <iostream>
#include "Archiver.hxx"

namespace DBricks {

class SexpArchiver : public Archiver {
public:
    SexpArchiver(std::ostream& stream)
        :m_stream(stream)
    {
    }
    
    virtual void object_begin(const std::string& name)
    {
        m_stream << "(make-" << name << '\n';
    }
    
    virtual void object_end(const std::string& name)
    {
        m_stream << ")\n";
    }

    virtual void list_begin(const std::string& name)
    {
        m_stream << "(list " << '\n';
    }

    virtual void list_end(const std::string& name)
    {
        m_stream << ")\n";
    }
    
    virtual void serialize(const std::string& name, double value)
    {
        serialize_internal(name, value);
    }
    
    virtual void serialize(const std::string& name, int    value)
    {
        serialize_internal(name, value);
    }
private:
    template<class ValueT>
    void serialize_internal(const std::string& name, ValueT value)
    {
        m_stream << ":" << name << ' ' << value << '\n';
    }
    
    std::ostream& m_stream;
};

} // namespace DBricks


#endif // SEXPARCHIVER_HPP
