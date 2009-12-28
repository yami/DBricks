#ifndef MENU_HXX
#define MENU_HXX

#include <vector>

#include <util/bit.hxx>
#include <util/stl.hxx>
#include <util/assert.hxx>

namespace DBricks {


class MenuAction {
public:
    // TODO: do it in better way...
    virtual void operator () (void* object) = 0;
};

// TODO: change to a more generic name instead of ShapeMenuAction
template<class ShapeT, class MethodT>
class ShapeMenuAction : public MenuAction {
public:
    ShapeMenuAction(MethodT method)
        :m_method(method)
    {
    }

    virtual void operator () (void* shape)
    {
        ShapeT* specific_shape = reinterpret_cast<ShapeT*>(shape);
        ASSERT(specific_shape);

        (specific_shape->*m_method)();
    }
private:
    MethodT m_method;
};

class MenuItem {
public:
    typedef unsigned int FlagsType;
    
    static const FlagsType Menu_Item_None       = 0x0;

    MenuItem (const std::string& name, const std::string& text, MenuAction* action, FlagsType flags=Menu_Item_None)
        :m_name(name), m_text(text), m_action(action), m_flags(flags)
    {
    }

    void operator () (void* object) const
    {
        (*m_action)(object);
    }

    std::string name() const
    {
        return m_name;
    }
    
    std::string text() const
    {
        return m_text;
    }
    
    bool is_set(FlagsType flag) const
    {
        return bit_is_set(m_flags, flag);
    }

    bool is_clr(FlagsType flag) const
    {
        return bit_is_clr(m_flags, flag);
    }

    void set(FlagsType flag)
    {
        bit_set(m_flags, flag);
    }
private:
    std::string m_name;
    std::string m_text;
    MenuAction* m_action;
    FlagsType   m_flags;
};


class Menu {
public:
    typedef std::vector<MenuItem*> MenuItemsType;    

    Menu ()
    {
    }

    ~Menu ()
    {
        util::delete_entries(m_items.begin(), m_items.end());
    }
    
    MenuItemsType& items()
    {
        return m_items;
    }
    
    size_t size() const
    {
        return m_items.size();
    }

    Menu& append(MenuItem* item)
    {
        m_items.push_back(item);
        return *this;
    }
    
    MenuItem* operator [] (size_t index) const
    {
        return m_items[index];
    }
private:
    MenuItemsType m_items;
};

} // namespace DBricks


#endif // MENU_HXX
