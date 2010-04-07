#include "Shape.hxx"

namespace DBricks {

ShapeTypeInventory theInventory;

ShapeType::ShapeType(const std::string& name)
    :m_name(name), m_short_name(name), m_collection(0)
{
}

const std::string& ShapeType::name() const
{
    ASSERT(m_collection);
    return m_name;
}

void ShapeType::join(ShapeTypeCollection* c)
{
    ASSERT(!m_collection);
    m_name = "/" + c->name() + "/" + m_name;
    m_collection = c;
}

void ShapeTypeCollection::add(ShapeType* shape_type)
{
    shape_type->join(this);
    m_shape_types.push_back(shape_type);
}


void ShapeTypeInventory::add_collection(ShapeTypeCollection* collection)
{    
    m_collections.push_back(collection);

    ShapeTypeCollection::ShapeTypesType& shape_types = collection->shape_types();
    for (ShapeTypeCollection::ShapeTypesType::iterator iter = shape_types.begin();
         iter != shape_types.end();
         ++iter) {
        ASSERT(m_shape_type_map.find((*iter)->name()) == m_shape_type_map.end());
        m_shape_type_map[(*iter)->name()] = (*iter);
    }
}

ShapeType* ShapeTypeInventory::shape_type(const std::string& name) const
{
    ShapeTypeMapType::const_iterator iter = m_shape_type_map.find(name);

    return iter != m_shape_type_map.end() ? (*iter).second : 0;
}

ShapeType* lookup_shape_type(const std::string& name)
{
    return theInventory.shape_type(name);
}

} // namespace DBricks

