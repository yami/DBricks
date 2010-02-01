#ifndef HISTORY_HXX
#define HISTORY_HXX

namespace DBricks {

class Change;

class History {
public:
    Change* remove_change()
    {
        Change* change = m_changes.back();
        m_changes.resize(m_changes.size() - 1);
        return change;
    }    
    
    void append(Change* change)
    {
        m_changes.push_back(change);
    }

    bool empty() const
    {
        return m_changes.empty();
    }
private:
    std::vector<Change*> m_changes;
};

} // namespace DBricks


#endif // HISTORY_HXX
