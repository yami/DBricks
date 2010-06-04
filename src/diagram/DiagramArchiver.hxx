#ifndef DIAGRAMARCHIVER_HXX
#define DIAGRAMARCHIVER_HXX

#include <map>
#include <string>

#include <sml/Sml.hxx>
#include "Shape.hxx"
#include "ShapeFactory.hxx"

namespace DBricks {

class DiagramArchiver {
public:
    Sml::Object* object() const
    {
        return m_object;
    }
    
    void* object(Sml::Object* new_object)
    {
        m_object = new_object;
    }

    void save_shape(Sml::Object* object, Shape* shape)
    {
        Sml::Object* saved = m_object;
        m_object = object;

        object->add_attribute_data(":id",   (void*) shape);
        object->add_attribute_data(":type", shape->type()->name());

        shape->save(this);
            
        m_object = saved;
    }

    Shape* load_shape(Sml::Object* object)
    {
        std::string  shape_type    = object->get_attribute_string(":type");
        void*        shape_id      = object->get_attribute_pointer(":id");
        Shape* shape = ShapeFactory::create_shape(shape_type);

        Sml::Object* saved = m_object;
        m_object = object;
        
        shape->load(this);

        m_object = saved;

        m_id_shape_map[shape_id] = shape;

        return shape;
    }

    void add_connection(void* id, int ihandle, void* to_shape, int to_index)
    {
        m_connection_info.push_back(ConnectionInfo(id, ihandle, to_shape, to_index));
    }

    void build_connections()
    {
        for (size_t i = 0; i < m_connection_info.size(); ++i) {
            Shape* shape1 = m_id_shape_map[m_connection_info[i].id];
            Shape* shape2 = m_id_shape_map[m_connection_info[i].to_shape];

            Connector* connector1 = shape1->connector(m_connection_info[i].ihandle);
            Connector* connector2 = shape2->connector(m_connection_info[i].to_index);

            Connector::build_connections(connector1, connector2);
        }
    }
private:
    struct ConnectionInfo {
        ConnectionInfo(void* i, int h, void* s, int idx)
            :id(i), ihandle(h), to_shape(s), to_index(idx)
        {
        }
                       
        void* id;
        int   ihandle;
        void* to_shape;
        int   to_index;
    };
    
    Sml::Object* m_object;
    std::vector<ConnectionInfo> m_connection_info;
    std::map<void*, Shape*> m_id_shape_map;
};

} // namespace DBricks


#endif // DIAGRAMARCHIVER_HXX
