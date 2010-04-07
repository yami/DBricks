#ifndef SHAPE_HXX
#define SHAPE_HXX

#include <map>
#include <vector>
#include <cmath>
#include <geom/Point.hxx>
#include <geom/Rect.hxx>
#include <cairomm/context.h>

#include <util/bit.hxx>

#include "Handle.hxx"
#include "Connector.hxx"
#include "IWithProperties.hxx"


namespace Sml {
class Object;
}


namespace DBricks {

class Connector;
class Menu;
class PropertyMap;
class PropertyDescriptor;
class IRenderer;

class Shape : public IWithProperties {
public:
    typedef std::vector<Handle*>    HandlesType;
    typedef std::vector<Connector*> ConnectorsType;
    typedef unsigned int            FlagsType;

    static const FlagsType Break_Connections = 0x1;
    
    Shape(FlagsType flags=0)
        :m_flags(flags), m_corner(0, 0),
         m_show_handles(false), m_show_connectors(false),
         m_property_map(0), m_property_descriptor(0)
    {
    }

    Shape(double x, double y, FlagsType flags=0)
        :m_flags(flags), m_corner(x, y),
         m_show_handles(false), m_show_connectors(false),
         m_property_map(0), m_property_descriptor(0)
    {
    }
    
    ~Shape()
    {
    }

    void draw (IRenderer* renderer) const
    {
        draw_shape(renderer);

        if (m_show_handles) {
            for (HandlesType::const_iterator iter = m_handles.begin();
                 iter != m_handles.end();
                 ++iter) {
                (*iter)->draw(renderer);
            }
        }

        if (m_show_connectors) {
            for (ConnectorsType::const_iterator iter = m_connectors.begin();
                 iter != m_connectors.end();
                 ++iter) {
                (*iter)->draw(renderer);
            }
        }
    }

    virtual void move_handle(Handle* handle, const Point& delta) = 0;
    virtual void move_connector(Connector* connector, const Point& delta) = 0;    
    virtual void move(const Point& delta) = 0;

    virtual double distance(const Point& point) const
    {
        return fabs(m_corner.x - point.x) + fabs(m_corner.y - point.y);
    }

    virtual bool cover(const Point& point) const = 0;
    virtual bool in(const Rect& rect) const = 0;
    virtual Rect bb() const = 0;

    virtual Menu* menu(const Point& point) const
    {
        return 0;
    }

    virtual Shape* clone() const
    {
        ASSERT_NOT_REACHED();
        return 0;
    }
    
    HandlesType& handles()
    {
        return m_handles;
    }

    ConnectorsType& connectors()
    {
        return m_connectors;
    }

    bool break_connections() const
    {
        return bit_is_set(m_flags, Break_Connections);
    }
    
    const Point& corner() const
    {
        return m_corner;
    }

    void show_handles()
    {
        m_show_handles = true;
    }

    void hide_handles()
    {
        m_show_handles = false;
    }

    void show_connectors()
    {
        m_show_connectors = true;
    }

    void hide_connectors()
    {
        m_show_connectors = false;
    }

    virtual void save (Sml::Object* object) const = 0;
    virtual void load (Sml::Object* object) = 0;

    virtual Gtk::Widget* property_widget() = 0;
    virtual void         property_apply() = 0;
    
private:
    virtual void draw_shape(IRenderer* renderer) const = 0;
protected:
    HandlesType m_handles;
    ConnectorsType m_connectors;

    FlagsType m_flags;
    
    Point m_corner;
    bool  m_show_handles;
    bool  m_show_connectors;

    PropertyMap*        m_property_map;
    PropertyDescriptor* m_property_descriptor;
};

class ShapeTypeCollection;

class ShapeType {
public:
    ShapeType(const std::string& name);

    const std::string& name() const;
    const std::string& short_name() const;
    
    void join(ShapeTypeCollection* c);
    
    virtual Shape* create(const Point& start, Handle*& handle) = 0;
    virtual Shape* create() = 0;

private:
    std::string          m_name;
    std::string          m_short_name;
    ShapeTypeCollection* m_collection;
};


#define DEFINE_SHAPE_TYPE(SHAPE, EXTERN, NAME)              \
    static ShapeTypeGeneric<SHAPE> EXTERN##_Object(NAME);   \
    ShapeType* EXTERN = &EXTERN##_Object                    \
        

template<class ShapeT>
class ShapeTypeGeneric : public ShapeType {
public:
    ShapeTypeGeneric(const std::string& name)
        :ShapeType(name)
    {
    }

    virtual ShapeT* create(const Point& start, Handle*& handle)
    {
        return new ShapeT(start, handle);
    }

    virtual ShapeT* create()
    {
        return new ShapeT();
    }
};

class ShapeTypeCollection {
public:
    typedef std::vector<ShapeType*> ShapeTypesType;
    
    ShapeTypeCollection(const std::string& name)
        :m_name(name)
    {
    }
    
    const std::string& name() const
    {
        return m_name;
    }
    
    void add(ShapeType* shape_type);

    ShapeTypesType& shape_types()
    {
        return m_shape_types;
    }
    
private:
    std::string    m_name;
    ShapeTypesType m_shape_types;
};


class ShapeTypeInventory {
public:
    typedef std::vector<ShapeTypeCollection*> CollectionsType;
    typedef std::map<std::string, ShapeType*> ShapeTypeMapType;
    
    void add_collection(ShapeTypeCollection* collection);
    ShapeType* shape_type(const std::string& name) const;
private:
    CollectionsType   m_collections;
    ShapeTypeMapType  m_shape_type_map;
};

ShapeType* lookup_shape_type(const std::string& name);

} // namespace DBricks

#endif // SHAPE_HXX
