#ifndef SHAPE_HXX
#define SHAPE_HXX

#include <map>
#include <vector>
#include <cmath>
#include <geom/Point.hxx>
#include <geom/Rect.hxx>

#include <cairomm/context.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/menu.h>


#include <util/bit.hxx>
#include <util/assert.hxx>


#include "Handle.hxx"
#include "Connector.hxx"
#include "IWithProperties.hxx"


namespace Sml {
class Object;
}

namespace Gtk {
class ActionGroup;
}

namespace DBricks {

class Connector;
class Menu;
class PropertyMap;
class PropertyDescriptor;
class IRenderer;
class ShapeType;
class DiagramArchiver;

class Shape : public IWithProperties {
public:
    typedef std::vector<Handle*>    HandlesType;
    typedef std::vector<Connector*> ConnectorsType;
    typedef unsigned int            FlagsType;

    static const FlagsType Break_Connections = 0x1;
    
    Shape(FlagsType flags=0)
        :m_flags(flags), m_corner(0, 0),
         m_show_handles(false), m_property_map(0), m_property_descriptor(0)
    {
    }

    Shape(double x, double y, FlagsType flags=0)
        :m_flags(flags), m_corner(x, y),
         m_show_handles(false), m_property_map(0), m_property_descriptor(0)
    {
    }
    
    ~Shape()
    {
    }

    void draw (IRenderer* renderer) const
    {
        draw_shape(renderer);

        for (ConnectorsType::const_iterator iter = m_connectors.begin();
             iter != m_connectors.end();
             ++iter) {
            (*iter)->draw(renderer);
        }        
        
        if (m_show_handles) {
            for (HandlesType::const_iterator iter = m_handles.begin();
                 iter != m_handles.end();
                 ++iter) {
                (*iter)->draw(renderer);
            }
        }
    }

    virtual void move_handle(Handle* handle, const Point& delta) = 0;
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

    virtual Shape* clone() const = 0;
    
    HandlesType& handles()
    {
        return m_handles;
    }

    ConnectorsType& connectors()
    {
        return m_connectors;
    }

    Connector* connector(size_t i) const
    {
        return m_connectors[i];
    }
    
    int connector_index(Connector* conn)
    {
        for (size_t i = 0; i<m_connectors.size(); ++i) {
            if (m_connectors[i] == conn)
                return i;
        }
        
        return -1;
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

    Handle* handle(const Point& point) const;

    virtual void save (DiagramArchiver* ar) const = 0;
    virtual void load (DiagramArchiver* ar) = 0;

    virtual Gtk::Widget* property_widget() = 0;
    virtual void         property_apply() = 0;

    virtual ShapeType*   type() const = 0;
    
private:
    virtual void draw_shape(IRenderer* renderer) const = 0;
protected:
    void init_copy(Shape* copy) const;
    
    HandlesType m_handles;
    ConnectorsType m_connectors;

    FlagsType m_flags;
    
    Point m_corner;
    bool  m_show_handles;

    PropertyMap*        m_property_map;
    PropertyDescriptor* m_property_descriptor;
};

class ShapeTypeCollection;

class ShapeType {
public:
    ShapeType(const std::string& name);

    const std::string name() const
    {
        return m_name;
    }
    
    const std::string short_name() const
    {
        return m_short_name;
    }
    
    void join(ShapeTypeCollection* c);
    
    virtual Shape* create(const Point& start, Handle*& handle) = 0;
    virtual Shape* create() = 0;

private:
    std::string          m_name;
    std::string          m_short_name;
    ShapeTypeCollection* m_collection;
};


#define DEFINE_SHAPE_TYPE(SHAPE, EXTERN, NAME)                  \
    static ShapeTypeGeneric<SHAPE> EXTERN##_Object(NAME);       \
    ShapeType* EXTERN = &EXTERN##_Object                        \
        

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


template<class ShapeT>
class ShapeTypeAbstract: public ShapeType {
public:
    ShapeTypeAbstract(const std::string& name)
        :ShapeType(name)
    {
    }

    virtual ShapeT* create(const Point& start, Handle*& handle)
    {
        ASSERT_NOT_REACHED();
        return 0;
    }

    virtual ShapeT* create()
    {
        ASSERT_NOT_REACHED();
        return 0;
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

    const ShapeTypesType& shape_types() const
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

    std::string ui_info(int nindent, const std::string& indent) const;

    template<class FuncT>
    void        action_group_init(Glib::RefPtr<Gtk::ActionGroup> ag, FuncT func) const;
private:
    CollectionsType   m_collections;
    ShapeTypeMapType  m_shape_type_map;
};


template<class FuncT>
void ShapeTypeInventory::action_group_init(Glib::RefPtr<Gtk::ActionGroup> ag, FuncT func) const
{
    for (CollectionsType::const_iterator iter = m_collections.begin();
         iter != m_collections.end();
         ++iter) {
        ag->add(Gtk::Action::create("Collection_" + (*iter)->name(), (*iter)->name()));                

        for (ShapeTypeCollection::ShapeTypesType::const_iterator siter = (*iter)->shape_types().begin();
             siter != (*iter)->shape_types().end();
             siter++) {
            ag->add(Gtk::Action::create("Shape_" + (*siter)->short_name(), (*siter)->short_name()),
                    sigc::bind(func, (*siter)->name()));
        }
    }
}

ShapeType* lookup_shape_type(const std::string& name);

} // namespace DBricks

#endif // SHAPE_HXX
