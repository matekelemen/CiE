#ifndef CIE_UTILS_BIT_OPERATIONS_IMPL_HPP
#define CIE_UTILS_BIT_OPERATIONS_IMPL_HPP


namespace cie::utils {


template <concepts::Integer IntegerType, concepts::Integer IndexType>
inline IntegerType
flipBit( IntegerType integer, IndexType bitIndex )
{
    return integer ^ (1 << bitIndex);
}


} // namespace cie::utils


#endif