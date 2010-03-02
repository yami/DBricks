#ifndef PROPERTYMAP_HXX
#define PROPERTYMAP_HXX

#include "Property.hxx"
#include "IWithProperties.hxx"


#include <vector>
#include <string>

namespace DBricks {

class PropertyMap {
public:
    PropertyMap(IWithProperties* object)
        :m_object(object)
    {
    }
    
    void add(Property* property)
    {
        m_map.push_back(property);
    }

    Property* get(const std::string& id) const
    {
        for (std::vector<Property*>::const_iterator iter = m_map.begin();
             iter != m_map.end();
             ++iter) {
            if ((*iter)->id() == id)
                return *iter;
        }

        return 0;
    }
    
    void save() const
    {
        for (std::vector<Property*>::const_iterator iter = m_map.begin();
             iter != m_map.end();
             ++iter) {
            (*iter)->save_to_value();
        }

        m_object->property_apply();
    }

    void load() const
    {
        for (std::vector<Property*>::const_iterator iter = m_map.begin();
             iter != m_map.end();
             ++iter) {
            (*iter)->load_to_widget();
        }
    }
    
private:
    IWithProperties*       m_object;
    std::vector<Property*> m_map;
};



} // namespace DBricks


#endif // PROPERTYMAP_HXX
