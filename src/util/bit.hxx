#ifndef BIT_HXX
#define BIT_HXX

template<class ValueT>
inline void bit_zero(ValueT& flags)
{
    flags = (ValueT)0;
}

template<class ValueT>
inline void bit_one(ValueT& flags)
{
    flags = (ValueT)~0;
}

template<class ValueT>
inline void bit_set(ValueT& flags, const ValueT& mask)
{
    flags |= mask;
}

template<class ValueT>
inline void bit_clr(ValueT& flags, const ValueT& mask)
{
    flags &= ~mask;
}

template<class ValueT>
inline bool bit_is_set(const ValueT& flags, const ValueT& mask)
{
    return flags & mask;
}

template<class ValueT>
inline bool bit_is_clr(const ValueT& flags, const ValueT& mask)
{
    return !(flags & mask);
}

#endif // BIT_HXX
