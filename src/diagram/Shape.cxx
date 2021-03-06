#include "Shape.hxx"

#include <logging/logging.hxx>

namespace DBricks {

ShapeTypeInventory theInventory;


void
Shape::init_copy(Shape* copy) const
{
    copy->m_flags = m_flags;

    for (HandlesType::const_iterator iter = m_handles.begin();
         iter != m_handles.end();
         ++iter) {
        copy->m_handles.push_back((*iter)->clone(copy));
    }

    for (ConnectorsType::const_iterator iter = m_connectors.begin();
         iter != m_connectors.end();
         ++iter) {
        copy->m_connectors.push_back((*iter)->clone(copy));
    }    
}


Handle* Shape::handle(const Point& point) const
{
    for (HandlesType::const_iterator iter = m_handles.begin();
         iter != m_handles.end();
         ++iter) {

        DLOG(DIAGRAM, DEBUG, "handle={%g, %g}, point={%g, %g}\n", (*iter)->point().x, (*iter)->point().y, point.x, point.y);
        if ((*iter)->point() == point)
            return *iter;
    }

    return 0;
}

ShapeType::ShapeType(const std::string& name)
    :m_name(name), m_short_name(name), m_collection(0)
{
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

std::string ShapeTypeInventory::ui_info(int nindent, const std::string& indent_string) const
{
    std::string indent;
    std::stringstream os;

    for (int i = 0; i < nindent; i++)
        indent += indent_string;
    
    for (CollectionsType::const_iterator iter = m_collections.begin();
         iter != m_collections.end();
         ++iter) {
        os << indent << "<menu action='Collection_" << (*iter)->name() << "' >\n";
        for (ShapeTypeCollection::ShapeTypesType::const_iterator siter = (*iter)->shape_types().begin();
             siter != (*iter)->shape_types().end();
             siter++) {
            os << indent << indent_string << "<menuitem action='Shape_" << (*siter)->short_name() << "' />\n";
        }
        os << indent << "</menu>\n";
    }

    return os.str();
}


ShapeType* lookup_shape_type(const std::string& name)
{
    return theInventory.shape_type(name);
}

} // namespace DBricks

