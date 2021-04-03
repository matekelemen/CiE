#ifndef CIE_LINALG_TYPES_VECTOR_WRAPPER_IMPL_HPP
#define CIE_LINALG_TYPES_VECTOR_WRAPPER_IMPL_HPP


namespace cie::linalg {


template <class WT, class VT, class IT, class CIT>
VectorWrapper<WT,VT,IT,CIT>::VectorWrapper( const VectorWrapper<WT,VT,IT,CIT>& r_rhs ) :
    _wrapped( r_rhs )
{
}


template <class WT, class VT, class IT, class CIT>
VectorWrapper<WT,VT,IT,CIT>::VectorWrapper( VectorWrapper<WT,VT,IT,CIT>&& r_rhs ) :
    _wrapped( std::move(r_rhs) )
{
}


template <class WT, class VT, class IT, class CIT>
VectorWrapper<WT,VT,IT,CIT>::VectorWrapper( const VectorWrapper<WT,VT,IT,CIT>&& r_rhs ) :
    _wrapped( std::move(r_rhs) )
{
}


template <class WT, class VT, class IT, class CIT>
template <class ...Args>
VectorWrapper<WT,VT,IT,CIT>::VectorWrapper( Args&&... r_args ) :
    _wrapped( std::forward<Args>(args)... )
{
}


template <class WT, class VT, class IT, class CIT>
typename VectorWrapper<WT,VT,IT,CIT>::iterator
VectorWrapper<WT,VT,IT,CIT>::begin()
{
    return &this->_wrapped( 0 );
}


} // namespace cie::linalg


#endif