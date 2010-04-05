#ifndef CHANGE_HXX
#define CHANGE_HXX

#include <vector>
#include <algorithm>

namespace DBricks {

class Change {
public:
    virtual ~Change() {}
    
    virtual void apply() = 0;
    virtual void revert() = 0;
};

class ChangeSet {
public:
    void append(Change* change)
    {
        m_changes.push_back(change);
    }
    
    virtual void apply()
    {
        std::for_each(m_changes.begin(), m_changes.end(), std::mem_fun(&Change::apply));
    }
    virtual void revert()
    {
        std::for_each(m_changes.begin(), m_changes.end(), std::mem_fun(&Change::revert));
    }
private:
    std::vector<Change*> m_changes;
};

} // namespace DBricks


#endif // CHANGE_HXX
