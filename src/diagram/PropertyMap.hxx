#ifndef PROPERTYMAP_HXX
#define PROPERTYMAP_HXX

#include "Property.hxx"

#include <vector>
#include <string>

namespace DBricks {

class PropertyMap {
public:
    void add(Property* property)
    {
        m_map.push_back(property);
    }

    Property* get(const std::string& id) const
    {
        for (std::vector<Property*>::iterator iter = m_map.begin();
             iter != m_map.end();
             ++iter) {
            if ((*iter)->id() == id)
                return *iter;
        }

        return 0;
    }
    
    void on_apply()
    {
        for (std::vector<Property*>::iterator iter = m_map.begin();
             iter != m_map.end();
             ++iter) {
            (*iter)->on_apply();
        }
    }
    
private:
    std::vector<Property*> m_map;
};



} // namespace DBricks


#endif // PROPERTYMAP_HXX
