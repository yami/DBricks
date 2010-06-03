#ifndef SML_HXX
#define SML_HXX

#include <string>
#include <map>
#include <vector>
#include <cstring>
#include <cassert>
#include <iostream>

// Simple Markup Language Interfaces
// the format reassembles JSON, that is:
//   JSON        SML
//   Array       List
//   Object      Object
//   Value       Value
//   Pair        Attribute


namespace Sml {

class List;
class Object;

enum DataType {
    DT_Unknown,
    DT_Bool,
    DT_Int,
    DT_Double,
    DT_String,
    DT_Object,
    DT_List,
    DT_Pointer,
};

std::string data_type_to_string(DataType vtype);
DataType   string_to_data_type(const std::string& ctype);

inline DataType data_type_enum(bool v)
{
    return DT_Bool;
}

inline DataType data_type_enum(int v)
{
    return DT_Int;
}

inline DataType data_type_enum(double v)
{
    return DT_Double;
}

inline DataType data_type_enum(std::string v)
{
    return DT_String;
}

inline DataType data_type_enum(const char* v)
{
    return DT_String;
}

inline DataType data_type_enum(Object* v)
{
    return DT_Object;
}

inline DataType data_type_enum(List* v)
{
    return DT_List;
}

inline DataType data_type_enum(void* v)
{
    return DT_Pointer;
}


class Value {
public:
    template<class DataT>
    Value(DataT v)
        :m_type(data_type_enum(v))
    {
        set(v);
    }

    ~Value()
    {
        if (m_type == DT_String)
            delete[] m_string;
    }
#define DEFINE_SML_VALUE_ACCESSOR(TYPE, VALUE)          \
    void get(TYPE& VALUE##_value) const {               \
        VALUE##_value = m_##VALUE;                      \
    }                                                   \
    void set(TYPE VALUE##_value) {                      \
        m_##VALUE = VALUE##_value;                      \
    }                                                   \
    TYPE get_##VALUE() const {                          \
        return m_##VALUE;                               \
    }
    
    DEFINE_SML_VALUE_ACCESSOR(bool   , bool)
    DEFINE_SML_VALUE_ACCESSOR(int    , int)
    DEFINE_SML_VALUE_ACCESSOR(double , double)
    DEFINE_SML_VALUE_ACCESSOR(Object*, object)
    DEFINE_SML_VALUE_ACCESSOR(List*  , list)
    DEFINE_SML_VALUE_ACCESSOR(void*  , pointer);
    
    void get(std::string& string_value)
    {
        string_value = std::string(m_string);
    }

    void set(const std::string& string_value)
    {
        m_string = new char[string_value.size()+1];
        std::strcpy(m_string, string_value.c_str());
    }

    void set(const char* string_value)
    {
        set(std::string(string_value));
    }

    std::string get_string() const
    {
        return std::string(m_string);
    }
    
    DataType type () const
    {
        return m_type;
    }
private:
    DataType m_type;
    
    union {
        bool        m_bool;
        int         m_int;
        double      m_double;
        char*       m_string;
        Object*     m_object;
        List*       m_list;
        void*       m_pointer;
    };
};



class Attribute {
public:
    Attribute(const std::string& name, Value* value)
        :m_name(name), m_value(value)
    {
    }

    std::string name() const
    {
        return m_name;
    }

    Value* value() const
    {
        return m_value;
    }

    DataType type() const
    {
        return m_value->type();
    }
    
    template<class DataT>
    void get(DataT& data)
    {
        m_value->get(data);
    }

    template<class DataT>
    void set(const DataT& data)
    {
        m_value->set(data);
    }

#define DEFINE_SML_ATTR_ACCESSOR(TYPE, VALUE)   \
    TYPE get_##VALUE() const {                  \
        return m_value->get_##VALUE();          \
    }

    DEFINE_SML_ATTR_ACCESSOR(bool        , bool)
    DEFINE_SML_ATTR_ACCESSOR(int         , int)
    DEFINE_SML_ATTR_ACCESSOR(double      , double)
    DEFINE_SML_ATTR_ACCESSOR(Object*     , object)
    DEFINE_SML_ATTR_ACCESSOR(List*       , list)
    DEFINE_SML_ATTR_ACCESSOR(std::string , string)
    DEFINE_SML_ATTR_ACCESSOR(void*       , pointer)

private:
    std::string m_name;
    Value*      m_value;
};


class Object {
public:
    typedef std::map<std::string, Attribute*> AttributesType;
    
    template<class DataT>
    void add_attribute_data(const std::string& name, const DataT& data)
    {
        Value* value = new Value(data);
        m_attrs[name] = new Attribute(name, value);
    }

    template<class DataT>
    void get_attribute_data(const std::string& name, DataT& data)
    {
        assert(m_attrs.find(name) != m_attrs.end());
        m_attrs[name]->get(data);
    }

#define DEFINE_SML_OBJECT_ACCESSOR(TYPE, VALUE)                         \
    TYPE get_attribute_##VALUE(const std::string& name) const {         \
        AttributesType::const_iterator iter = m_attrs.find(name);       \
        assert(iter != m_attrs.end());                                  \
        return iter->second->get_##VALUE();                             \
    }

    DEFINE_SML_OBJECT_ACCESSOR(bool        , bool)
    DEFINE_SML_OBJECT_ACCESSOR(int         , int)
    DEFINE_SML_OBJECT_ACCESSOR(double      , double)
    DEFINE_SML_OBJECT_ACCESSOR(Object*     , object)
    DEFINE_SML_OBJECT_ACCESSOR(List*       , list)
    DEFINE_SML_OBJECT_ACCESSOR(std::string , string)
    DEFINE_SML_OBJECT_ACCESSOR(void*       , pointer);
    
    void add_attribute_value(const std::string& name, Value* value)
    {
        m_attrs[name] = new Attribute(name, value);
    }

    void get_attribute_value(const std::string& name, Value*& value)
    {
        assert(m_attrs.find(name) != m_attrs.end());
        m_attrs[name]->value();
    }

    void add_attribute(Attribute* attr)
    {
        m_attrs[attr->name()] = attr;
    }
    
    Attribute* get_attribute(const std::string& name)
    {
        assert(m_attrs.find(name) != m_attrs.end());
        return m_attrs[name];
    }

    const AttributesType& attributes() const
    {
        return m_attrs;
    }
private:
    AttributesType m_attrs;
};


class List {
public:
    typedef std::vector<Value*> ValuesType;

    void add_value(Value* value)
    {
        m_values.push_back(value);
    }

    const ValuesType& values() const
    {
        return m_values;
    }
private:
    ValuesType m_values;
};

} // namespace Sml

#endif // SML_HXX
