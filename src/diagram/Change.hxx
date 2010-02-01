#ifndef CHANGE_HXX
#define CHANGE_HXX

namespace DBricks {

class Change {
public:
    virtual ~Change() {}
    
    virtual void apply() = 0;
    virtual void revert() = 0;
};

} // namespace DBricks


#endif // CHANGE_HXX
